#pragma once

#include "Common.h"
#include "Math.h"
#include "Timing.h"

#include "Input.h"
#include "Window.h"
#include "Renderer.h"
#include "Camera.h"
#include "Meshes.h"

#include <vector>

namespace Objects
{
	class GameObject
	{
	public:
		GameObject();
		~GameObject();

		virtual void Create();
		virtual void Destroy();

		virtual void Update();
		virtual void Draw();

		void UpdateTranform();

		void SetPosition(float x, float y, float z);
		void SetPosition(Math::Vector3F v) { SetPosition(v.x, v.y, v.z); }
		void SetScale(float width, float height, float depth);
		void SetScale(Math::Vector3F v) { SetScale(v.x, v.y, v.z); }
		void SetRotate(float radians, float axisX, float axisY, float axisZ);
		void SetRotate(float radians, Math::Vector3F v) { SetRotate(radians, v.x, v.y, v.z); };

		void Translate(float x, float y, float z);
		void Translate(Math::Vector3F v) { Translate(v.x, v.y, v.z); }
		void Scale(float width, float height, float depth);
		void Scale(Math::Vector3F v) { Scale(v.x, v.y, v.z); }
		void Rotate(float radians, float axisX, float axisY, float axisZ);
		void Rotate(float radians, Math::Vector3F v) { Rotate(radians, v.x, v.y, v.z); };

		void AddChild(GameObject& child);
		void RemoveChild(GameObject& child);

		GameObject* GetParent() { return m_parent; }
		GameObject* GetChild(int index) { return m_children[index]; }
		int GetChildCount() { return m_children.size(); }

		Math::Matrix4F GetLocalTransform() { return m_localTransform; }
		Math::Matrix4F GetGlobalTransform() { return m_globalTransform; }

		Math::Vector3F GetTranslation() 
		{ 
			return Math::Vector3F(m_globalTransform.m03, m_globalTransform.m13, m_globalTransform.m23); 
		}
		Math::Vector3F GetScale() 
		{ 
			float xAxis = Math::Vector3F(m_globalTransform.m00, m_globalTransform.m01, m_globalTransform.m02).Magnitude();
			float yAxis = Math::Vector3F(m_globalTransform.m10, m_globalTransform.m11, m_globalTransform.m12).Magnitude();
			float zAxis = Math::Vector3F(m_globalTransform.m20, m_globalTransform.m21, m_globalTransform.m22).Magnitude();
			return Math::Vector3F(xAxis, yAxis, zAxis);
		}
		Math::Vector3F GetRotation() 
		{ 
			float xAxis = atan2f(m_globalTransform.m12, m_globalTransform.m11);
			float yAxis = atan2f(m_globalTransform.m20, m_globalTransform.m00);
			float zAxis = atan2f(m_globalTransform.m01, m_globalTransform.m00);
			return Math::Vector3F(xAxis, yAxis, zAxis); 
		}

	protected:
		GameObject* m_parent;
		std::vector<GameObject*> m_children;

		Math::Matrix4F m_localTransform;
		Math::Matrix4F m_globalTransform;

	};
	class CameraObject : public GameObject
	{
	public:
		CameraObject();
		~CameraObject();

		void Create(Window& window);
		void Destroy() override;

		void Update() override;
		void Draw() override;

		Renderer::Camera& GetCameraRenderer() { return m_camera; }

	public:
		float turnSpeed = 0.0012f;
		float moveSpeed = 0.8f;

		float yaw = 0.0f;
		float pitch = 0.0f;

	private:
		Renderer::Camera m_camera;

	};
	class ModelObject : public GameObject
	{
	public:
		ModelObject();
		~ModelObject();

		void Create(Renderer::Renderer& renderer, const char* modelPath);
		void Destroy() override;

		void Update() override;
		void Draw() override;

		void SetCamera(CameraObject* camera) { m_camera = camera; }

	private:
		Renderer::Meshes::MeshRenderer m_meshRenderer;
		CameraObject* m_camera;

	};
}
