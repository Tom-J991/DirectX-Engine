#include "Camera.h"

namespace Renderer
{
	Camera::Camera()
		: m_type(CameraTypes::NONE)
		, m_forward({0.0f, 0.0f, -1.0f})
	{ }
	Camera::Camera(Math::Vector3F forward)
		: m_type(CameraTypes::NONE)
		, m_forward(forward)
	{ }
	Camera::~Camera()
	{ }

	void Camera::CreatePerspective(Window& window, float fov, float zNear, float zFar)
	{
		m_type = CameraTypes::PERSPECTIVE;
		m_fov = fov;
		m_zNear = zNear;
		m_zFar = zFar;

		// Get Apsect Ratio
		int windowWidth, windowHeight;
		float aspectRatio;
		{
			RECT clientRect;
			GetClientRect(window.GetWindowHandle(), &clientRect);
			windowWidth = clientRect.right - clientRect.left;
			windowHeight = clientRect.bottom - clientRect.top;
			aspectRatio = (float)windowWidth / (float)windowHeight;
		}

		// Setup Perspective Matrix
		m_projectionMat = Math::Matrix4F(1.0f);
		m_projectionMat.Perspective(aspectRatio, Math::DegreesToRadians(fov), zNear, zFar);
	}
	void Camera::CreateOrthographic(Window& window, float scale, float zNear, float zFar)
	{
		m_type = CameraTypes::ORTHOGRAPHIC;
		m_scale = scale;
		m_zNear = zNear;
		m_zFar = zFar;

		// Setup Orthographic Matrix.
		m_projectionMat = Math::Matrix4F(1.0f);
		m_projectionMat.Orthographic(scale, 0.0f, 1.0f, 0.0f, 1.0f, zNear, zFar);
	}
	void Camera::Destroy()
	{ }

	void Camera::Update()
	{ }
	void Camera::Draw(Math::Matrix4F& viewMat)
	{
		if (m_type != CameraTypes::NONE)
		{
			static Math::Vector3F worldUp = { 0.0f, 1.0f, 0.0f }; // Y Up Constant
			m_viewMat = &viewMat;
			// Calculate Forward and Right vectors.
			m_forward = { -viewMat.m[2][0], -viewMat.m[2][1], -viewMat.m[2][2] };
			m_right = worldUp.Cross(m_forward).Normalise();
		}
	}
}
