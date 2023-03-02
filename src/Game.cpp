#include "Game.h"

void Game::Create()
{
	KeyboardInput.Create();
	MouseInput.Create();
	m_window.Create();
	m_renderer.Create(m_window);
	m_audioEngine.Create();
	{
		// Create here..
		m_camera = Renderer::Camera({ 0, .35f, .1f }, 0.0f, 0.0f);
		m_camera.CreatePerspective(m_window, 75.0f, 0.01f, 1000.0f);

		m_player.Create(m_renderer);
		m_player.LoadModel("./res/models/heavy.obj");

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
		m_player.Destroy();
		m_camera.Destroy();

		m_audioEngine.UnloadAudio("./res/sounds/test.ogg");
		m_audioEngine.UnloadAudio("./res/sounds/music/streamed/rivaldealer.ogg");
	}
	m_audioEngine.Destroy();
}

float cameraSpeed = 0.0012f;
float playerSpeed = 0.8f;
void Game::Update()
{
	KeyboardInput.Update();
	{
		// Update here..
		// 
		if (KeyboardInput.IsKeyUp(VK_ESCAPE))
			Destroy(); // Quit Game
		// FPS Camera
		static float lastMouseX = 0.0f, lastMouseY = 0.0f;
		m_camera.SetYaw(m_camera.GetYaw() + ((MouseInput.x - lastMouseX) * cameraSpeed));
		m_camera.SetPitch(m_camera.GetPitch() + ((MouseInput.y - lastMouseY) * cameraSpeed));
		lastMouseX = MouseInput.x; lastMouseY = MouseInput.y;
		int roll = KeyboardInput.IsKeyPressed('E') - KeyboardInput.IsKeyPressed('Q');
		m_camera.SetRoll(m_camera.GetRoll() + (cameraSpeed * 16.0f) * roll);
		// Constrain Camera Pitch
		if (m_camera.GetPitch() >= Math::DegreesToRadians(89.0f))
			m_camera.SetPitch(Math::DegreesToRadians(89.0f));
		if (m_camera.GetPitch() <= Math::DegreesToRadians(-89.0f))
			m_camera.SetPitch(Math::DegreesToRadians(-89.0f));
		if (KeyboardInput.IsKeyDown('R'))
		{
			// Reset
			m_camera.SetYaw(0); m_camera.SetPitch(0); m_camera.SetRoll(0);
		}
		// FPS Movement
		Math::Vector3F move = Math::Vector3F();
		int moveX = KeyboardInput.IsKeyPressed('A') - KeyboardInput.IsKeyPressed('D');
		int moveY = KeyboardInput.IsKeyPressed(VK_SPACE) - KeyboardInput.IsKeyPressed(VK_CONTROL);
		int moveZ = KeyboardInput.IsKeyPressed('W') - KeyboardInput.IsKeyPressed('S');
		move = m_camera.GetRight() * (float)moveX + m_camera.GetForward() * (float)moveZ;
		move.y = (float)moveY;
		m_camera.SetPosition(m_camera.GetPosition() + move.Normalise() * playerSpeed * GameTime.GetDelta());
	}
	m_camera.Update();
	m_audioEngine.Update();
}

bool showImGui = true;
void Game::Draw()
{
	m_renderer.BeginFrame();
	{
		ImGui::Begin("Camera", &showImGui);
		{ // ImGui
			ImGui::Text("Position: { X:%.3f, Y:%.3f, Z:%.3f }", m_camera.GetPosition().x, m_camera.GetPosition().y, m_camera.GetPosition().z);
			ImGui::Text("Rotation: { Pitch:%.3f, Yaw:%.3f, Roll:%.3f }", m_camera.GetPitch(), m_camera.GetYaw(), m_camera.GetRoll());
		}
		ImGui::End();
		// Draw here..
		//
		m_camera.Draw();
		m_player.Draw(m_camera);
	}
	m_renderer.EndFrame();
}
