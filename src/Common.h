#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <memory>
#include <assert.h>

#include "external/imgui/imgui.h"
#include "external/imgui/backends/imgui_impl_win32.h"
#include "external/imgui/backends/imgui_impl_dx11.h"

#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")

#define WINDOW_CLASS "WindowClass"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_ASPECT (WINDOW_WIDTH/WINDOW_HEIGHT)
#define WINDOW_FPS 60

#define MOUSE_LOCK true
