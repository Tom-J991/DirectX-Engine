#include "Camera.h"

namespace Renderer
{
	Camera::Camera()
		: m_type(CameraTypes::NONE)
		, m_position({0.0f, 0.0f, 0.0f})
		, m_forward({0.0f, 0.0f, -1.0f})
		, m_pitch(0.0f)
		, m_yaw(0.0f)
		, m_roll(0.0f)
	{ }
	Camera::Camera(Math::Vector3F position, float pitch, float yaw)
		: m_type(CameraTypes::NONE)
		, m_position(position)
		, m_forward({ 0.0f, 0.0f, -1.0f })
		, m_pitch(pitch)
		, m_yaw(yaw)
		, m_roll(0.0f)
	{ }
	Camera::Camera(Math::Vector3F position, float pitch, float yaw, float roll)
		: m_type(CameraTypes::NONE)
		, m_position(position)
		, m_forward({ 0.0f, 0.0f, -1.0f })
		, m_pitch(pitch)
		, m_yaw(yaw)
		, m_roll(roll)
	{ }
	Camera::Camera(Math::Vector3F position, Math::Vector3F forward, float pitch, float yaw)
		: m_type(CameraTypes::NONE)
		, m_position(position)
		, m_forward(forward)
		, m_pitch(pitch)
		, m_yaw(yaw)
		, m_roll(0.0f)
	{ }
	Camera::Camera(Math::Vector3F position, Math::Vector3F forward, float pitch, float yaw, float roll)
		: m_type(CameraTypes::NONE)
		, m_position(position)
		, m_forward(forward)
		, m_pitch(pitch)
		, m_yaw(yaw)
		, m_roll(roll)
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
		m_projectionMat = m_projectionMat.Perspective(aspectRatio, Math::DegreesToRadians(fov), zNear, zFar);
	}
	void Camera::CreateOrthographic(Window& window, float scale, float zNear, float zFar)
	{
		m_type = CameraTypes::ORTHOGRAPHIC;
		m_scale = scale;
		m_zNear = zNear;
		m_zFar = zFar;

		// Setup Orthographic Matrix.
		m_projectionMat = Math::Matrix4F(1.0f);
		m_projectionMat = m_projectionMat.Orthographic(scale, 0.0f, 1.0f, 0.0f, 1.0f, zNear, zFar);
	}
	void Camera::Destroy()
	{ }

	void Camera::Update()
	{ }
	void Camera::Draw()
	{
		if (m_type != CameraTypes::NONE)
		{
			Math::Vector3F worldUp = { 0.0f, 1.0f, 0.0f }; // Y Up Constant
			// Set View Matrix
			m_viewMat = Math::Matrix4F(1.0f);
			m_viewMat = m_viewMat.SetTranslation(-m_position).RotateZ(-m_roll).RotateY(-m_yaw).RotateX(-m_pitch);
			// Calculate Forward and Right vectors.
			m_forward = { -m_viewMat.m[2][0], -m_viewMat.m[2][1], -m_viewMat.m[2][2] };
			m_right = worldUp.Cross(m_forward).Normalise();
		}
	}
}
