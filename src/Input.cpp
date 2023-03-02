#include "Input.h"

namespace Input
{
	void Keyboard::Create()
	{ }
	void Keyboard::Destroy()
	{ }

	void Keyboard::Update()
	{
		m_keyEvent = ReadKey();
	}

	bool Keyboard::IsKeyPressed(char key)
	{
		return m_keyStates[key] == true;
	}
	bool Keyboard::IsKeyReleased(char key)
	{
		return m_keyStates[key] == false;
	}

	bool Keyboard::IsKeyDown(char key)
	{
		return m_keyEvent.IsPressed() && m_keyEvent.GetKeyCode() == key;
	}
	bool Keyboard::IsKeyUp(char key)
	{
		return m_keyEvent.IsReleased() && m_keyEvent.GetKeyCode() == key;
	}

	bool Keyboard::KeyBufferEmpty()
	{
		return m_keyBuffer.empty();
	}
	bool Keyboard::CharBufferEmpty()
	{
		return m_charBuffer.empty();
	}

	KeyboardEvent Keyboard::ReadKey()
	{
		if (m_keyBuffer.empty())
			return KeyboardEvent();
		else
		{
			KeyboardEvent e = m_keyBuffer.front();
			m_keyBuffer.pop();
			return e;
		}
	}
	unsigned char Keyboard::ReadChar()
	{
		if (m_charBuffer.empty())
			return 0u;
		else
		{
			unsigned char e = m_charBuffer.front();
			m_charBuffer.pop();
			return e;
		}
	}

	void Keyboard::OnKeyPressed(const unsigned char key)
	{
		m_keyStates[key] = true;
		m_keyBuffer.push(KeyboardEvent(KeyboardEvent::Pressed, key));
	}
	void Keyboard::OnKeyReleased(const unsigned char key)
	{
		m_keyStates[key] = false;
		m_keyBuffer.push(KeyboardEvent(KeyboardEvent::Released, key));
	}
	void Keyboard::OnChar(const unsigned char key)
	{
		m_charBuffer.push(key);
	}

	KeyboardEvent::KeyboardEvent()
		: type(EventType::Invalid)
		, key(0u)
	{ }
	KeyboardEvent::KeyboardEvent(const EventType type, const unsigned char key)
		: type(type)
		, key(key)
	{ }

	bool KeyboardEvent::IsPressed() const
	{
		return type == EventType::Pressed;
	}
	bool KeyboardEvent::IsReleased() const
	{
		return type == EventType::Released;
	}
	bool KeyboardEvent::IsValid() const
	{
		return type != EventType::Invalid;
	}

	unsigned char KeyboardEvent::GetKeyCode() const
	{
		return key;
	}

	void Mouse::Create()
	{ }
	void Mouse::Destroy()
	{ }
}
