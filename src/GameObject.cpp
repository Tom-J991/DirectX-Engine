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
		m_meshRenderer.SetModelMatrix(&m_globalTransform);
		m_meshRenderer.LoadModel(modelPath);
		OutputDebugString("Created");
	}
	void ModelObject::Destroy()
	{
		GameObject::Destroy();
		m_meshRenderer.Destroy();
	}

	void ModelObject::Update()
	{
		GameObject::Update();
		//OutputDebugString("Updating");
	}
	void ModelObject::Draw()
	{
		GameObject::Draw();
		m_meshRenderer.Draw(*m_camera);
		//OutputDebugString("Drawing");
	}
}
