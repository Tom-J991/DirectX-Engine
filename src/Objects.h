#pragma once

#include "Common.h"
#include "Math.h"
#include "Renderer.h"
#include "Camera.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>
#include <map>

namespace Renderer
{
	namespace Objects
	{
		struct TexVertex2D
		{
			union {
				struct { float x, y; };
				float pos[2];
			};
			union {
				struct { float u, v; };
				float uv[2];
			};
		};
		struct Vertex3D
		{
			union {
				struct { float x, y, z; };
				float pos[3];
			};
			union {
				struct { float r, g, b; };
				float color[3];
			};
		};
		struct TexVertex3D
		{
			union {
				struct { float x, y, z; };
				float pos[3];
			};
			union {
				struct { float u, v; };
				float uv[2];
			};
		};

		struct Texture
		{
			unsigned int m_id;
			const char* m_type;
			const char* m_texFilePath;

			ID3D11SamplerState* m_samplerState;
			ID3D11Texture2D* m_texture;
			ID3D11ShaderResourceView* m_textureView;

			Texture LoadTexture(Renderer& renderer, const char* filePath);

			void Use(Renderer& renderer, unsigned int id);
		};
		struct Shader
		{
			ID3D11VertexShader* m_vertexShader;
			ID3D11PixelShader* m_pixelShader;
			ID3D11InputLayout* m_inputLayout;

			void Setup(Renderer& renderer, LPCWSTR sPath);
			void Use(Renderer& renderer);
			void Destroy();
		};
		struct Mesh
		{
			std::vector<TexVertex3D> m_vertices;
			std::vector<uint16_t> m_indices;
			Shader m_shader;

			UINT m_vertexCount;
			UINT m_indexCount;
			UINT m_stride;
			UINT m_offset;
			aiMesh* m_mesh;

			ID3D11Buffer* m_vertexBuffer;
			ID3D11Buffer* m_indexBuffer;	

			void Setup(Renderer& renderer, LPCWSTR shaderPath);
			void Draw(Renderer& renderer);
			void Destroy();
		};

		class TexturedModel
		{
		public:
			TexturedModel();
			~TexturedModel();

			void Create(Renderer& renderer);
			void Destroy();

			void LoadModel(const char* filePath, const wchar_t* shaderPath = L"./res/shaders/modelShader.hlsl");

			void Draw(Camera& camera);

			const char* GetModelPath() { return m_modelFilePath; }
			Math::Matrix4F GetModelMatrix() { return m_modelMat; }
			void SetModelMatrix(Math::Matrix4F mat) { m_modelMat = mat; }

		private:
			Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
			void ProcessNode(aiNode* node, const aiScene* scene);

		private:
			Renderer m_renderer;

			std::vector<Texture> m_textures;
			std::vector<Texture> m_loadedTextures;
			UINT m_textureCount = 0;

			std::vector<Mesh> m_meshes;
			UINT m_meshCount = 0;
			const char* m_modelFilePath;

			ID3D11Buffer* m_constantBuffer;

			Math::Matrix4F m_modelMat;
			Math::Matrix4F m_modelViewProj;
		};
	}
}
