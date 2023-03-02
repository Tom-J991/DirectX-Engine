#include "Renderer.h"

namespace Renderer
{
	Renderer::Renderer()
		: m_swapChain(nullptr)
		, m_device(nullptr)
		, m_deviceContext(nullptr)
		, m_frameBuffer(nullptr)
		, m_depthBuffer(nullptr)
		, m_rasterizerState(nullptr)
		, m_depthStencilState(nullptr)
		, m_infoQueue(nullptr)
	{ }
	Renderer::~Renderer()
	{ }

	void Renderer::Create(Window& window)
	{
		m_window = window;
		InitDX();

		// ImGui
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		//io.WantCaptureMouse = true;
		//io.WantCaptureKeyboard = true;
		io.IniFilename = 0; // Disable Config
		(void)io;
		ImGui::StyleColorsLight();
		ImGui_ImplWin32_Init(window.GetWindowHandle());
		ImGui_ImplDX11_Init(m_device, m_deviceContext);
	}
	void Renderer::Destroy()
	{
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();

		DestroyDX();
	}

	void Renderer::BeginFrame(void)
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		float clearColor[4] = { 0.0f, 0.2f, 0.4f, 1.0f };
		m_deviceContext->ClearRenderTargetView(m_frameBuffer, clearColor);
		m_deviceContext->ClearDepthStencilView(m_depthBuffer, D3D11_CLEAR_DEPTH, 1.0f, 0);

		m_deviceContext->RSSetState(m_rasterizerState);
		m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 0);

		RECT winRect;
		GetClientRect(m_window.GetWindowHandle(), &winRect);
		D3D11_VIEWPORT viewport = { 
			0.0f, 
			0.0f, 
			(float)(winRect.right - winRect.left),
			(float)(winRect.bottom - winRect.top),
			0.0f,
			1.0f
		};
		m_deviceContext->RSSetViewports(1, &viewport);
		m_deviceContext->OMSetRenderTargets(1, &m_frameBuffer, m_depthBuffer);
	}
	void Renderer::EndFrame(void)
	{
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		m_swapChain->Present(1, 0);
	}

	void Renderer::InitDX()
	{
		// Create DX Devices and Context
		{
			ID3D11Device* baseDevice;
			ID3D11DeviceContext* baseDeviceContext;
			D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };
			UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if defined(DEBUG_BUILD)
			creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

			HRESULT result = D3D11CreateDevice(
				0,
				D3D_DRIVER_TYPE_HARDWARE,
				0,
				0,
				nullptr,
				0,
				D3D11_SDK_VERSION,
				&baseDevice,
				0,
				&baseDeviceContext
			);
			if (FAILED(result))
			{
				MessageBox(0, "D3D11CreateDevice error", "Error", MB_OK);
				OutputDebugString("D3D11CreateDevice error: ");
				OutputDebugString((LPCSTR)GetLastError());
				OutputDebugString("\n");
				return;
			}

			result = baseDevice->QueryInterface(__uuidof(ID3D11Device1), (LPVOID*)&m_device);
			assert(SUCCEEDED(result));
			baseDevice->Release();

			result = baseDeviceContext->QueryInterface(__uuidof(ID3D11DeviceContext1), (LPVOID*)&m_deviceContext);
			assert(SUCCEEDED(result));
			baseDeviceContext->Release();
		}

		// DX Debug Layer
#ifdef DEBUG_BUILD
		ID3D11Debug* debug = nullptr;
		m_device->QueryInterface(__uuidof(ID3D11InfoQueue), (LPVOID*)&m_infoQueue);
		if (m_infoQueue)
		{
			ID3D11InfoQueue* infoQueue = nullptr;
			if (SUCCEEDED(m_infoQueue->QueryInterface(__uuidof(ID3D11InfoQueue), (LPVOID*)&infoQueue)))
			{
				infoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
				infoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
				infoQueue->Release();
			}
			m_infoQueue->Release();
		}
#endif

		// Create Swap Chain
		{
			// Get Factory for Swap Chain
			IDXGIFactory2* dxFactory;
			{
				IDXGIDevice1* dxDevice;
				HRESULT result = m_device->QueryInterface(__uuidof(IDXGIDevice1), (LPVOID*)&dxDevice);
				assert(SUCCEEDED(result));

				IDXGIAdapter* dxAdapter;
				result = dxDevice->GetAdapter(&dxAdapter);
				assert(SUCCEEDED(result));
				dxDevice->Release();

				DXGI_ADAPTER_DESC adapterDesc;
				dxAdapter->GetDesc(&adapterDesc);

				OutputDebugString("Graphics Device: ");
				OutputDebugStringW(adapterDesc.Description);

				result = dxAdapter->GetParent(__uuidof(IDXGIFactory2), (LPVOID*)&dxFactory);
				assert(SUCCEEDED(result));
			}

			DXGI_SWAP_CHAIN_DESC1 swapChainDesc;
			ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC1));
			swapChainDesc.Width = 0;
			swapChainDesc.Height = 0;
			swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			swapChainDesc.SampleDesc.Count = 1;
			swapChainDesc.SampleDesc.Quality = 0;
			swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapChainDesc.BufferCount = 2;
			swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
			swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
			swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
			swapChainDesc.Flags = 0;

			HRESULT result = dxFactory->CreateSwapChainForHwnd(
				m_device, 
				m_window.GetWindowHandle(),
				&swapChainDesc,
				0,
				0,
				&m_swapChain
			);
			assert(SUCCEEDED(result));
			dxFactory->Release();
		}

		// Create Frame Buffer
		{
			ID3D11Texture2D* frameBuffer;
			HRESULT result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&frameBuffer);
			assert(SUCCEEDED(result));

			result = m_device->CreateRenderTargetView(frameBuffer, 0, &m_frameBuffer);
			assert(SUCCEEDED(result));

			D3D11_TEXTURE2D_DESC depthBufferDesc;
			frameBuffer->GetDesc(&depthBufferDesc);
			frameBuffer->Release();
			depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

			ID3D11Texture2D* depthBuffer;
			m_device->CreateTexture2D(&depthBufferDesc, nullptr, &depthBuffer);
			result = m_device->CreateDepthStencilView(depthBuffer, nullptr, &m_depthBuffer);
			assert(SUCCEEDED(result));
			depthBuffer->Release();
		}

		// Create States
		{
			D3D11_RASTERIZER_DESC rasterizerDesc;
			ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
			rasterizerDesc.FillMode = D3D11_FILL_SOLID;
			rasterizerDesc.CullMode = D3D11_CULL_BACK;
			rasterizerDesc.FrontCounterClockwise = true;
			m_device->CreateRasterizerState(&rasterizerDesc, &m_rasterizerState);

			D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
			ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
			depthStencilDesc.DepthEnable = true;
			depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
			depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
			m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);
		}
	}
	void Renderer::DestroyDX()
	{
		m_swapChain->Release();
		m_depthStencilState->Release();
		m_rasterizerState->Release();
		m_depthBuffer->Release();
		m_frameBuffer->Release();
		m_device->Release();
		m_deviceContext->Release();
#if defined(DEBUG_BUILD)
		m_infoQueue->Release();
#endif
	}
}
