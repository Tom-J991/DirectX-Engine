#include "Objects.h"

#include <string>
#include <math.h>

#define STB_IMAGE_IMPLEMENTATION
#include "external/stb_image.h"

struct Constants
{
	Math::Matrix4F modelViewProj;
};

namespace Renderer
{
	namespace Objects
	{
		TexturedModel::TexturedModel()
			: m_constantBuffer(nullptr)
		{ }
		TexturedModel::~TexturedModel()
		{ }

		void TexturedModel::Create(Renderer& renderer)
		{
			m_renderer = renderer;
			m_modelMat = Math::Matrix4F(1.0f);
		}
		void TexturedModel::Destroy()
		{
			for (auto m : m_meshes)
				m.Destroy();
			m_constantBuffer->Release();
		}

		Mesh TexturedModel::ProcessMesh(aiMesh* mesh, const aiScene* scene)
		{
			std::vector<TexVertex3D> meshData;
			std::vector<uint16_t> indiceData;
			Mesh m;
			// Vertices
			{
				for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
				{
					TexVertex3D vertex;
					vertex.x = mesh->mVertices[i].x;
					vertex.y = mesh->mVertices[i].y;
					vertex.z = mesh->mVertices[i].z;
					if (mesh->mTextureCoords[0])
					{
						vertex.u = mesh->mTextureCoords[0][i].x;
						vertex.v = mesh->mTextureCoords[0][i].y;
					}
					else
					{
						vertex.u = 0.0f;
						vertex.v = 0.0f;
					}
					meshData.push_back(vertex);
				}
				m.m_vertices = meshData;
				m.m_vertexCount = meshData.size();
			}
			// Indices
			{
				for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
				{
					aiFace face = mesh->mFaces[i];
					for (unsigned int j = 0; j < face.mNumIndices; ++j)
					{
						indiceData.push_back(face.mIndices[j]);
					}
				}
				m.m_indices = indiceData;
				m.m_indexCount = indiceData.size();
			}
			// Textures
			{
				if (mesh->mMaterialIndex >= 0)
				{
					aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
					auto type = aiTextureType_DIFFUSE;
					auto tc = material->GetTextureCount(type);
					if (tc > 0)
					{
						for (unsigned int i = 0; i < tc; i++)
						{
							aiString str;
							material->GetTexture(type, i, &str);
							Texture tex = tex.LoadTexture(m_renderer, str.C_Str());
							tex.m_id = m_textureCount;
							tex.m_type = "texture_diffuse";
							tex.m_texFilePath = str.C_Str();
							m_textures.push_back(tex);
							m_textureCount++;
						}
					}
				}
			}
			m.m_stride = sizeof(TexVertex3D);
			m.m_offset = 0;
			m.m_mesh = mesh;
			m_meshCount++;

			return m;
		}
		void TexturedModel::ProcessNode(aiNode* node, const aiScene* scene)
		{
			for (unsigned int i = 0; i < node->mNumMeshes; ++i)
			{
				aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
				m_meshes.push_back(ProcessMesh(mesh, scene));
			}
			for (unsigned int i = 0; i < node->mNumChildren; ++i)
			{
				ProcessNode(node->mChildren[i], scene);
			}
		}

		void TexturedModel::LoadModel(const char* filePath, const wchar_t* shaderPath)
		{
			m_modelFilePath = filePath;
			// Load Model
			{
				Assimp::Importer importer;
				const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs);
				ProcessNode(scene->mRootNode, scene);

				for (unsigned int i = 0; i < m_meshes.size(); i++)
				{
					auto& m = m_meshes[i];
					m.Setup(m_renderer, shaderPath);
				}
			}
			// Create Constant Buffer
			{
				D3D11_BUFFER_DESC constantBufferDesc;
				ZeroMemory(&constantBufferDesc, sizeof(D3D11_BUFFER_DESC));
				constantBufferDesc.ByteWidth = sizeof(Constants) + 0xf & 0xfffffff0;
				constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
				constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
				constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
				HRESULT result = m_renderer.GetDevice()->CreateBuffer(&constantBufferDesc, nullptr, &m_constantBuffer);
				assert(SUCCEEDED(result));
			}
		}

		void TexturedModel::Draw(Camera& camera)
		{
			// Camera
			m_modelViewProj = m_modelMat * camera.GetViewMatrix() * camera.GetProjectionMatrix();

			// Update Constant Buffer
			{
				D3D11_MAPPED_SUBRESOURCE mappedSubresource;
				m_renderer.GetDeviceContext()->Map(m_constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);
				Constants* constants = (Constants*)(mappedSubresource.pData);
				constants->modelViewProj = m_modelViewProj;
				m_renderer.GetDeviceContext()->Unmap(m_constantBuffer, 0);
			}
			// Draw Mesh
			for (unsigned int i = 0; i < m_meshes.size(); i++)
			{
				auto& m = m_meshes[i];
				m.m_shader.Use(m_renderer);
				if (m_textureCount > 0)
				{
					auto& tex = m_textures[i];
					tex.Use(m_renderer, tex.m_id);
				}
				m_renderer.GetDeviceContext()->VSSetConstantBuffers(0, 1, &m_constantBuffer);
				m.Draw(m_renderer);
			}
		}

		void Shader::Setup(Renderer& renderer, LPCWSTR sPath)
		{
			// Create Shaders
			{
				// Vertex Shader
				ID3DBlob* vsBlob;
				{
					ID3DBlob* shaderCompileErrors;
					HRESULT result = D3DCompileFromFile(
						sPath,
						nullptr,
						nullptr,
						"vs_main",
						"vs_5_0",
						0,
						0,
						&vsBlob,
						&shaderCompileErrors
					);
					if (FAILED(result))
					{
						const char* errorString = nullptr;
						if (result == HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND))
							errorString = "D3DCompileFromFile error: File not found.";
						else if (shaderCompileErrors)
						{
							errorString = (const char*)shaderCompileErrors->GetBufferPointer();
							shaderCompileErrors->Release();
						}
						MessageBox(0, errorString, "Error", MB_OK);
						OutputDebugString("D3DCompileFromFile error: ");
						OutputDebugString(errorString);
						OutputDebugString("\n");
						return;
					}

					result = renderer.GetDevice()->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &m_vertexShader);
					assert(SUCCEEDED(result));
				}
				// Pixel Shader
				ID3DBlob* psBlob;
				{
					ID3DBlob* shaderCompileErrors;
					HRESULT result = D3DCompileFromFile(
						sPath,
						nullptr,
						nullptr,
						"ps_main",
						"ps_5_0",
						0,
						0,
						&psBlob,
						&shaderCompileErrors
					);
					if (FAILED(result))
					{
						const char* errorString = nullptr;
						if (result == HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND))
							errorString = "D3DCompileFromFile error: File not found.";
						else if (shaderCompileErrors)
						{
							errorString = (const char*)shaderCompileErrors->GetBufferPointer();
							shaderCompileErrors->Release();
						}
						MessageBox(0, errorString, "Error", MB_OK);
						OutputDebugString("D3DCompileFromFile error: ");
						OutputDebugString(errorString);
						OutputDebugString("\n");
						return;
					}

					result = renderer.GetDevice()->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &m_pixelShader);
					assert(SUCCEEDED(result));
				}
				psBlob->Release();
				// Create Input Layout
				{
					D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
					{
						{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
						{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
					};

					HRESULT result = renderer.GetDevice()->CreateInputLayout(inputElementDesc, ARRAYSIZE(inputElementDesc), vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &m_inputLayout);
					assert(SUCCEEDED(result));
				}
				vsBlob->Release();
			}
		}
		void Shader::Use(Renderer& renderer)
		{
			renderer.GetDeviceContext()->IASetInputLayout(m_inputLayout);
			renderer.GetDeviceContext()->VSSetShader(m_vertexShader, nullptr, 0);
			renderer.GetDeviceContext()->PSSetShader(m_pixelShader, nullptr, 0);
		}
		void Shader::Destroy()
		{
			m_vertexShader->Release();
			m_pixelShader->Release();
			m_inputLayout->Release();
		}

		Texture Texture::LoadTexture(Renderer& renderer, const char* filePath)
		{
			m_texFilePath = filePath;
			m_type = "texture_diffuse";
			// Create Sampler State
			{
				HRESULT result;

				D3D11_SAMPLER_DESC samplerDesc;
				ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
				samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
				samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
				samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
				samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
				samplerDesc.MipLODBias = 0.0f;
				samplerDesc.MaxAnisotropy = 1;
				samplerDesc.BorderColor[0] = 1.0f;
				samplerDesc.BorderColor[1] = 1.0f;
				samplerDesc.BorderColor[2] = 1.0f;
				samplerDesc.BorderColor[3] = 1.0f;
				samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
				samplerDesc.MinLOD = -FLT_MAX;
				samplerDesc.MaxLOD = FLT_MAX;
				result = renderer.GetDevice()->CreateSamplerState(&samplerDesc, &m_samplerState);
				assert(SUCCEEDED(result));
			}
			// Load Image and Create Texture
			{
				HRESULT result;

				int texWidth, texHeight, texNumChannels;
				int texForceNumChannels = 4;
				unsigned char* textureBytes = stbi_load(filePath, &texWidth, &texHeight, &texNumChannels, texForceNumChannels);
				//assert(textureBytes);
				int texBytesPerRow = 4 * sizeof(unsigned char) * texWidth;

				// Create Texture
				D3D11_TEXTURE2D_DESC textureDesc;
				ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));
				textureDesc.Width = texWidth;
				textureDesc.Height = texHeight;
				textureDesc.MipLevels = 1;
				textureDesc.ArraySize = 1;
				//textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
				textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				textureDesc.SampleDesc.Count = 1;
				textureDesc.SampleDesc.Quality = 0;
				textureDesc.Usage = D3D11_USAGE_DYNAMIC;
				textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
				textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

				D3D11_SUBRESOURCE_DATA textureSubresource;
				ZeroMemory(&textureSubresource, sizeof(D3D11_SUBRESOURCE_DATA));
				textureSubresource.pSysMem = textureBytes;
				textureSubresource.SysMemPitch = texBytesPerRow;
				result = renderer.GetDevice()->CreateTexture2D(&textureDesc, &textureSubresource, &m_texture);
				//assert(SUCCEEDED(result));

				D3D11_SHADER_RESOURCE_VIEW_DESC shaderResource;
				ZeroMemory(&shaderResource, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
				shaderResource.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				shaderResource.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
				shaderResource.Texture2D.MipLevels = 1;
				result = renderer.GetDevice()->CreateShaderResourceView(m_texture, &shaderResource, &m_textureView);
				//assert(SUCCEEDED(result));

				free(textureBytes); // Free Image
			}
			return Texture(m_id, m_type, m_texFilePath, m_samplerState, m_texture, m_textureView);
		}
		void Texture::Use(Renderer& renderer, unsigned int id)
		{
			renderer.GetDeviceContext()->PSSetShaderResources(0, 1, &m_textureView);
			renderer.GetDeviceContext()->PSSetSamplers(0, 1, &m_samplerState);
		}

		void Mesh::Setup(Renderer& renderer, LPCWSTR shaderPath)
		{
			m_shader.Setup(renderer, shaderPath);
			// Mesh Data
			{
				D3D11_BUFFER_DESC vertexBufferDesc;
				ZeroMemory(&vertexBufferDesc, sizeof(D3D11_BUFFER_DESC));
				vertexBufferDesc.ByteWidth = m_vertexCount * sizeof(TexVertex3D);
				vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
				vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

				D3D11_SUBRESOURCE_DATA vertexData;
				ZeroMemory(&vertexData, sizeof(D3D11_SUBRESOURCE_DATA));
				vertexData.pSysMem = &m_vertices[0];

				HRESULT result = renderer.GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
				assert(SUCCEEDED(result));

				D3D11_BUFFER_DESC indexBufferDesc;
				ZeroMemory(&indexBufferDesc, sizeof(D3D11_BUFFER_DESC));
				indexBufferDesc.ByteWidth = m_indexCount * sizeof(uint16_t);
				indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
				indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

				D3D11_SUBRESOURCE_DATA indexData;
				ZeroMemory(&indexData, sizeof(D3D11_SUBRESOURCE_DATA));
				indexData.pSysMem = &m_indices[0];

				result = renderer.GetDevice()->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
				assert(SUCCEEDED(result));
			}
		}
		void Mesh::Draw(Renderer& renderer)
		{
			renderer.GetDeviceContext()->IASetVertexBuffers(0, 1, &m_vertexBuffer, &m_stride, &m_offset);
			renderer.GetDeviceContext()->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R16_UINT, 0);
			renderer.GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			renderer.GetDeviceContext()->DrawIndexed(m_indexCount, 0, 0);
		}
		void Mesh::Destroy()
		{
			m_vertexBuffer->Release();
			m_indexBuffer->Release();
			m_shader.Destroy();
		}
	}
}
