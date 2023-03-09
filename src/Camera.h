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
		Camera(Math::Vector3F forward);
		~Camera();

		void CreatePerspective(Window& window, float fov, float zNear, float zFar);
		void CreateOrthographic(Window& window, float scale, float zNear, float zFar);
		void Destroy();
		 
		void Update();
		void Draw(Math::Matrix4F& viewMat);

		// Setters
		float SetFOV(float fov) { if (m_type == ORTHOGRAPHIC || m_type == NONE) return 0.0f; m_fov = fov; return this->m_fov; }
		float SetScale(float scale) { if (m_type == PERSPECTIVE || m_type == NONE) return 0.0f; m_scale = scale; return this->m_scale; }

		// Getters
		CameraTypes GetType() { return m_type; }

		Math::Vector3F GetForward() { return m_forward; }
		Math::Vector3F GetRight() { return m_right; }
		float GetFOV() { return m_fov; }
		float GetScale() { return m_scale; }
		float GetNear() { return m_zNear; }
		float GetFar() { return m_zFar; }

		Math::Matrix4F GetProjectionMatrix() { return m_projectionMat; }
		Math::Matrix4F GetViewMatrix() { return *m_viewMat; }

	private:
		CameraTypes m_type;

		Math::Vector3F m_forward;
		Math::Vector3F m_right;
		float m_fov; // Perspective Only
		float m_scale; // Orthographic Only
		float m_zNear;
		float m_zFar;

		Math::Matrix4F m_projectionMat;
		Math::Matrix4F* m_viewMat;

	};
}
