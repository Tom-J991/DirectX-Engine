#include "GameObject.h"

namespace Objects
{
	// Util
	bool RemoveElement(std::vector<GameObject*>& v, GameObject& obj)
	{
		auto e = v.erase(std::remove(v.begin(), v.end(), &obj), v.end());
		if (e == v.end())
			return true;

		return false;
	}
	// Game Object
	GameObject::GameObject()
		: m_parent(nullptr)
		, m_children()
	{ }
	GameObject::~GameObject()
	{ }

	void GameObject::Create()
	{
		m_localTransform = Math::Matrix4F(1.0f);
		m_globalTransform = Math::Matrix4F(1.0f);
	}
	void GameObject::Destroy()
	{
		if (m_parent != nullptr)
			m_parent->RemoveChild(*this);
		for (auto o : m_children)
			o->m_parent = nullptr;
	}

	void GameObject::Update()
	{
		for (auto o : m_children)
			o->Update();
	}
	void GameObject::Draw()
	{
		for (auto o : m_children)
			o->Draw();
	}

	void GameObject::UpdateTranform()
	{
		if (m_parent != nullptr)
			m_globalTransform = m_parent->GetGlobalTransform() * m_localTransform;
		else
			m_globalTransform = m_localTransform;

		for (auto o : m_children)
			o->UpdateTranform();
	}

	void GameObject::SetPosition(float x, float y, float z)
	{
		m_localTransform.SetTranslation(x, y, z);
		UpdateTranform();
	}
	void GameObject::SetScale(float width, float height, float depth)
	{
		m_localTransform.SetScaled(width, height, depth);
		UpdateTranform();
	}
	void GameObject::SetRotate(float radians, float axisX, float axisY, float axisZ)
	{
		m_localTransform.SetRotateX(radians * axisX);
		m_localTransform.SetRotateY(radians * axisY);
		m_localTransform.SetRotateZ(radians * axisZ);
		UpdateTranform();
	}

	void GameObject::Translate(float x, float y, float z)
	{
		m_localTransform.Translate(x, y, z);
		UpdateTranform();
	}
	void GameObject::Scale(float width, float height, float depth)
	{
		m_localTransform.Scale(width, height, depth);
		UpdateTranform();
	}
	void GameObject::Rotate(float radians, float axisX, float axisY, float axisZ)
	{
		m_localTransform.RotateX(radians * axisX);
		m_localTransform.RotateY(radians * axisY);
		m_localTransform.RotateZ(radians * axisZ);
		UpdateTranform();
	}

	void GameObject::AddChild(GameObject& child)
	{
		child.m_parent = this;
		m_children.push_back(&child);
	}
	void GameObject::RemoveChild(GameObject& child)
	{
		if (RemoveElement(m_children, child))
		{
			child.m_parent = nullptr;
		}
	}
	// Camera Object
	CameraObject::CameraObject()
		: GameObject()
	{ }
	CameraObject::~CameraObject()
	{ }

	void CameraObject::Create(Window& window)
	{
		GameObject::Create();
		m_camera = Renderer::Camera();
		m_camera.CreatePerspective(window, 75.0f, 0.01f, 1000.0f);
	}
	void CameraObject::Destroy()
	{
		GameObject::Destroy();
		m_camera.Destroy();
	}

	void CameraObject::Update()
	{
		// Looking
		static float lastMouseX = 0.0f, lastMouseY = 0.0f;
		yaw = (MouseInput.x - lastMouseX) * turnSpeed;
		pitch = (MouseInput.y - lastMouseY) * turnSpeed;
		lastMouseX = MouseInput.x; lastMouseY = MouseInput.y;
		// Constrain Camera Pitch
		if (pitch >= Math::DegreesToRadians(89.0f))
			pitch = Math::DegreesToRadians(89.0f);
		if (pitch <= Math::DegreesToRadians(-89.0f))
			pitch = Math::DegreesToRadians(-89.0f);
		if (KeyboardInput.IsKeyDown('R'))
		{
			// Reset View Rotation
			yaw = 0.0f; pitch = 0.0f;
		}
		// Walking
		Math::Vector3F move = Math::Vector3F();
		int moveX = KeyboardInput.IsKeyPressed('A') - KeyboardInput.IsKeyPressed('D');
		int moveY = KeyboardInput.IsKeyPressed(VK_SPACE) - KeyboardInput.IsKeyPressed(VK_CONTROL);
		int moveZ = KeyboardInput.IsKeyPressed('W') - KeyboardInput.IsKeyPressed('S');
		move = m_camera.GetRight() * (float)moveX + m_camera.GetForward() * (float)moveZ;
		move.y = (float)moveY;
		// Move
		Rotate(-yaw, { 0, 1, 0 });
		Rotate(-pitch, { 1, 0, 0 });
		Translate(-move.Normalise() * moveSpeed * GameTime.GetDelta());
		// Update
		m_camera.Update();
		GameObject::Update();
	}
	void CameraObject::Draw()
	{
		m_camera.Draw(m_globalTransform);
		GameObject::Draw();
	}
	// Model Object
	ModelObject::ModelObject()
		: GameObject()
	{ }
	ModelObject::~ModelObject()
	{ }

	void ModelObject::Create(Renderer::Renderer& renderer, const char* modelPath)
	{
		GameObject::Create();
		m_meshRenderer.Create(renderer);
		m_meshRenderer.LoadModel(modelPath);
	}
	void ModelObject::Destroy()
	{
		GameObject::Destroy();
		m_meshRenderer.Destroy();
	}

	void ModelObject::Update()
	{
		GameObject::Update();
	}
	void ModelObject::Draw()
	{
		Renderer::Camera* c = &m_camera->GetCameraRenderer();
		m_meshRenderer.Draw(m_globalTransform, *c);
		GameObject::Draw();
	}
}
