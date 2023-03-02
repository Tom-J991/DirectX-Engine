#pragma once

#include "Common.h"
#include "Math.h"
#include "Timing.h"
#include "Window.h"
#include "Renderer.h"

namespace Renderer
{
	typedef enum
	{
		NONE,
		PERSPECTIVE,
		ORTHOGRAPHIC,
		MAX
	} CameraTypes;

	class Camera
	{
	public:
		Camera();
		Camera(Math::Vector3F position, float pitch, float yaw);
		Camera(Math::Vector3F position, float pitch, float yaw, float roll);
		Camera(Math::Vector3F position, Math::Vector3F forward, float pitch, float yaw);
		Camera(Math::Vector3F position, Math::Vector3F forward, float pitch, float yaw, float roll);
		~Camera();

		void CreatePerspective(Window& window, float fov, float zNear, float zFar);
		void CreateOrthographic(Window& window, float scale, float zNear, float zFar);
		void Destroy();
		 
		void Update();
		void Draw();

		// Setters
		Math::Vector3F SetPosition(Math::Vector3F position) { m_position = position; return m_position; }
		Math::Vector3F SetPosition(float x, float y, float z) { m_position = {x, y, z}; return m_position; }

		float SetPitch(float pitch) { m_pitch = pitch; return this->m_pitch; }
		float SetYaw(float yaw) { m_yaw = yaw; return this->m_yaw; }
		float SetRoll(float roll) { m_roll = roll; return this->m_roll; }
		float SetFOV(float fov) { m_fov = fov; return this->m_fov; }

		// Getters
		CameraTypes GetType() { return m_type; }

		Math::Vector3F GetPosition() { return m_position; }
		Math::Vector3F GetForward() { return m_forward; }
		Math::Vector3F GetRight() { return m_right; }
		float GetPitch() { return m_pitch; }
		float GetYaw() { return m_yaw; }
		float GetRoll() { return m_roll; }
		float GetFOV() { return m_fov; }
		float GetNear() { return m_zNear; }
		float GetFar() { return m_zFar; }

		Math::Matrix4F GetProjectionMatrix() { return m_projectionMat; }
		Math::Matrix4F GetViewMatrix() { return m_viewMat; }

	private:
		CameraTypes m_type;

		Math::Vector3F m_position;
		Math::Vector3F m_forward;
		Math::Vector3F m_right;
		float m_pitch;
		float m_yaw;
		float m_roll;
		float m_fov; // Perspective Only
		float m_scale; // Orthographic Only
		float m_zNear;
		float m_zFar;

		Math::Matrix4F m_projectionMat;
		Math::Matrix4F m_viewMat;

	};
}
