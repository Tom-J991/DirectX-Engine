#pragma once

#include "Common.h"
#include "Window.h"

#include <d3d11_1.h>
#include <d3dcompiler.h>

namespace Renderer
{
	class Renderer
	{
	public:
		Renderer();
		~Renderer();

		void Create(Window& window);
		void Destroy();

		void BeginFrame(void);
		void EndFrame(void);

		IDXGISwapChain1* GetSwapChain(void) { return m_swapChain; }
		ID3D11RenderTargetView* GetFrameBuffer(void) { return m_frameBuffer; }
		ID3D11Device1* GetDevice(void) { return m_device; }
		ID3D11DeviceContext1* GetDeviceContext(void) { return m_deviceContext; }
		ID3D11InfoQueue* GetInfoQueue(void) { return m_infoQueue; }

	private:
		void InitDX();
		void DestroyDX();

	private:
		Window m_window;

		IDXGISwapChain1* m_swapChain;
		ID3D11Device1* m_device;
		ID3D11DeviceContext1* m_deviceContext;
		ID3D11InfoQueue* m_infoQueue;

		ID3D11RenderTargetView* m_frameBuffer;
		ID3D11DepthStencilView* m_depthBuffer;

		ID3D11RasterizerState* m_rasterizerState;
		ID3D11DepthStencilState* m_depthStencilState;

	};
}
