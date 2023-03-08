#pragma once

#include "Common.h"
#include "Math.h"

#include <vector>

class GameObject
{
public:
	GameObject();
	~GameObject();

	virtual void Update(float deltaTime);
	virtual void Draw();

	void UpdateTranform();

	void SetPosition(float x, float y);
	void SetPosition(Math::Vector2F v) { SetPosition(v.x, v.y); }
	void SetScale(float width, float height);
	void SetScale(Math::Vector2F v) { SetScale(v.x, v.y); }
	void SetRotate(float radians);

	void Translate(float x, float y);
	void Translate(Math::Vector2F v) { Translate(v.x, v.y); }
	void Scale(float width, float height);
	void Scale(Math::Vector2F v) { Scale(v.x, v.y); }
	void Rotate(float radians);

	void AddChild(GameObject& child);
	void RemoveChild(GameObject& child);

	GameObject GetParent() { return m_parent; }
	GameObject GetChild(int index) { return m_children[index]; }
	int GetChildCount() { return m_children.size(); }

	Math::Matrix3F GetLocalTransform() { return m_localTransform; }
	Math::Matrix3F GetGlobalTransform() { return m_globalTransform; }

protected:
	GameObject& m_parent;
	std::vector<GameObject> m_children;

	Math::Matrix3F m_localTransform;
	Math::Matrix3F m_globalTransform;

};
