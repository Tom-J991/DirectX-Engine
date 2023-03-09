#include "Game.h"

#include <string>
#include <sstream>

void Game::Create()
{
	KeyboardInput.Create();
	MouseInput.Create();
	m_window.Create();
	m_renderer.Create(m_window);
	m_audioEngine.Create();
	{
		// Create here..
		m_sceneRoot.Create();

		m_cameraObject.Create(m_window);
		m_cameraObject.SetPosition({ 0, 0, -1 });

		m_playerObject.Create(m_renderer, "./res/models/heavy.obj");
		m_playerObject.SetCamera(&m_cameraObject);

		// Add to Scene
		m_sceneRoot.AddChild(m_cameraObject);
		m_sceneRoot.AddChild(m_playerObject);

		m_audioEngine.PlayAudio("./res/sounds/test.ogg", 0.1f);
		//m_audioEngine.PlayMusic("./res/sounds/music/streamed/rivaldealer.ogg", 0.5f);
	}
	m_window.MessageLoop();
}
void Game::Destroy()
{
	m_window.Destroy();
	m_renderer.Destroy();
	MouseInput.Destroy();
	KeyboardInput.Destroy();
	{
		// Destroy here..
		m_playerObject.Destroy();
		m_cameraObject.Destroy();
		m_sceneRoot.Destroy();

		m_audioEngine.UnloadAudio("./res/sounds/test.ogg");
		m_audioEngine.UnloadAudio("./res/sounds/music/streamed/rivaldealer.ogg");
	}
	m_audioEngine.Destroy();
}

std::stringstream r;
void Game::Update()
{
	KeyboardInput.Update();
	{
		// Update here..
		// 
		if (KeyboardInput.IsKeyUp(VK_ESCAPE))
			Destroy(); // Quit Game

		//m_playerObject.Rotate(GameTime.GetDelta(), {0,1,0});
		m_sceneRoot.Update(); // Update Scene
	}
	m_audioEngine.Update();
}

bool showImGui = true;
void Game::Draw()
{
	m_renderer.BeginFrame();
	{
		ImGui::Begin("Camera", &showImGui);
		{ // ImGui
			ImGui::Text("Position: { X:%.3f, Y:%.3f, Z:%.3f }", 
				m_cameraObject.GetTranslation().x, 
				m_cameraObject.GetTranslation().y, 
				m_cameraObject.GetTranslation().z);
			ImGui::Text("Rotation: { Pitch:%.3f, Yaw:%.3f, Roll:%.3f }", 
				Math::RadiansToDegrees(m_cameraObject.GetRotation().x), 
				Math::RadiansToDegrees(m_cameraObject.GetRotation().y), 
				Math::RadiansToDegrees(m_cameraObject.GetRotation().z));
		}
		ImGui::End();
		// Draw here..
		//
		m_sceneRoot.Draw(); // Draw Scene
	}
	m_renderer.EndFrame();
}
