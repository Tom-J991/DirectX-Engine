#pragma once

#include "Common.h"
#include <Windows.h>
#include <windowsx.h>

#include <vector>

class Window
{
public:
	Window();
	~Window();

	void Create();
	void Destroy();

	void MessageLoop();

	HWND GetWindowHandle() { return m_windowHandle; }

private:
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	HWND m_windowHandle;

};
