#include "Window.h"
#include "Input.h"
#include "Game.h"
#include "Timing.h"

Window::Window()
{ }
Window::~Window()
{ }

void Window::Create()
{
	WNDCLASSEX wndClass;
	ZeroMemory(&wndClass, sizeof(WNDCLASSEX));
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = (CS_OWNDC);
	wndClass.lpfnWndProc = WindowProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = nullptr;
	wndClass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wndClass.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
	wndClass.hCursor = nullptr;
	wndClass.hbrBackground = nullptr;
	wndClass.lpszMenuName = nullptr;
	wndClass.lpszClassName = WINDOW_CLASS;
	if (!RegisterClassEx(&wndClass))
	{
		MessageBox(0, "RegisterClassEx error", "Error", MB_OK);
		OutputDebugString("RegisterClassEx error: ");
		OutputDebugString((LPCSTR)GetLastError());
		OutputDebugString("\n");
		return;
	}

	DWORD windowStyle = (WS_POPUP | WS_VISIBLE);
	RECT rect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
	AdjustWindowRect(&rect, windowStyle, false);

	m_windowHandle = CreateWindowEx(
		0,
		WINDOW_CLASS,
		"Game Window",
		windowStyle,
		CW_USEDEFAULT, CW_USEDEFAULT,
		rect.right - rect.left, 
		rect.bottom - rect.top,
		nullptr,
		nullptr,
		nullptr,
		nullptr
	);
	if (!m_windowHandle)
	{
		MessageBox(0, "CreateWindowEx error", "Error", MB_OK);
		OutputDebugString("CreateWindowEx error: ");
		OutputDebugString((LPCSTR)GetLastError());
		OutputDebugString("\n");
		return;
	}
	else
	{
		ShowWindow(m_windowHandle, SW_SHOW);
		UpdateWindow(m_windowHandle);

		static bool inputInit = false;
		// Raw Mouse Input
		{
			if (inputInit == false)
			{
				RAWINPUTDEVICE m_rawinputDevice;
				m_rawinputDevice.usUsagePage = 0x01;
				m_rawinputDevice.usUsage = 0x02;
				m_rawinputDevice.dwFlags = RIDEV_NOLEGACY;
				m_rawinputDevice.hwndTarget = 0;
				if (RegisterRawInputDevices(&m_rawinputDevice, 1, sizeof(m_rawinputDevice)) == FALSE)
				{
					OutputDebugString("RawInput Registration Error!\n");
				}
			}
		}
	}

	UnregisterClass(wndClass.lpszClassName, wndClass.hInstance);
}
void Window::Destroy()
{
	Timing::Instance().Destroy();
	DestroyWindow(m_windowHandle);
}

void Window::MessageLoop()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	Timing::Instance().Create();

	bool running = true;
	while (running)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if (msg.message == WM_QUIT)
				running = false;
		}
		else
		{
			// Game Loop
			Timing::Instance().CalculateTimings();
			Game::Instance().Update();
			Game::Instance().Draw();
		}
	}
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam); // ImGui
LRESULT Window::WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	RECT rect;
	GetClientRect(hWnd, &rect);
	MapWindowPoints(hWnd, nullptr, reinterpret_cast<POINT*>(&rect), 2);

	switch (msg)
	{
	case WM_INPUT:
	{
		// Get Raw Input
		UINT size;
		GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, NULL, &size, sizeof(RAWINPUTHEADER));
		if (size > 0)
		{
			std::unique_ptr<BYTE[]> rawData = std::make_unique<BYTE[]>(size);
			if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, rawData.get(), &size, sizeof(RAWINPUTHEADER)) == size)
			{
				RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(rawData.get());
				// Mouse Input
				if (raw->header.dwType == RIM_TYPEMOUSE)
				{
					if (raw->data.mouse.lLastX != 0 || raw->data.mouse.lLastY != 0)
					{
						// Get mouse relative coordinates.
						MouseInput.x += static_cast<float>(raw->data.mouse.lLastX);
						MouseInput.y += static_cast<float>(raw->data.mouse.lLastY);
						// Reset cursor back to window center.
						if (MOUSE_LOCK)
							SetCursorPos((rect.right - rect.left) / 2, (rect.bottom - rect.top) / 2);
					}
				}
			}
		}
	} return 0; break;
	case WM_CHAR:
	{
		unsigned char ch = static_cast<unsigned char>(wParam);
		const bool wasPressed = lParam & 0x40000000;
		if (!wasPressed)
		{
			KeyboardInput.OnChar(ch);
		}
	} return 0; break;
	case WM_KEYDOWN:
	{
		unsigned char key = static_cast<unsigned char>(wParam);
		const bool wasPressed = lParam & 0x40000000;
		if (!wasPressed)
		{
			KeyboardInput.OnKeyPressed(key);
		}
	} return 0; break;
	case WM_KEYUP:
	{
		unsigned char key = static_cast<unsigned char>(wParam);
		KeyboardInput.OnKeyReleased(key);
	} return 0; break;
	case WM_SYSCOMMAND:
	{
		if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT menu
			return 0;
	} return 0; break;
	case WM_ACTIVATE:
	{
		// Set cursor to window center.
		SetCursorPos((rect.right - rect.left)/2, (rect.bottom - rect.top)/2);
		// Clip Cursor to Window and Disable Cursor if enabled.
		if (MOUSE_LOCK)
		{
			if (wParam == WA_INACTIVE)
			{
				ClipCursor(nullptr);
				SetCursor((HCURSOR)IDC_ARROW);
			}
			else
			{
				ClipCursor(&rect);
				SetCursor(nullptr);
			}
		}
	} return 0; break;
	case WM_CLOSE:
	{
		DestroyWindow(hWnd);
	} return 0; break;
	case WM_DESTROY:
	{
		// Disable mouse clip and show cursor.
		if (MOUSE_LOCK)
		{
			ClipCursor(nullptr);
			SetCursor((HCURSOR)IDC_ARROW);
		}
		PostQuitMessage(0);
	} return 0; break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
