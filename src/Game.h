#pragma once

#include "Common.h"

#include "Math.h"
#include "Timing.h"

#include "Input.h"
#include "Window.h"
#include "Renderer.h"
#include "Audio.h"

#include "Camera.h"
#include "Objects.h"

class Game
{
public:
	void Create();
	void Destroy();

	void Update();
	void Draw();

private:
	Game() { }

	Window m_window;
	Renderer::Renderer m_renderer;
	AudioEngine::AudioEngine m_audioEngine;

	Renderer::Camera m_camera;
	Renderer::Objects::TexturedModel m_player;

public:
	// Singleton Design Pattern
	static Game& Instance()
	{
		static Game instance;
		return instance;
	}

	Game(Game const&)				= delete;
	void operator=(Game const&)		= delete;

};
