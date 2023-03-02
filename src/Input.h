#pragma once

#include "Common.h"

#include <queue>

#define KeyboardInput (Input::Keyboard::Instance())
#define MouseInput (Input::Mouse::Instance())
#define Keys (Input::KeyCodes)

namespace Input
{
	class KeyboardEvent
	{
	public:
		enum EventType
		{
			Pressed,
			Released,
			Invalid
		};

		KeyboardEvent();
		KeyboardEvent(const EventType type, const unsigned char key);
		
		bool IsPressed() const;
		bool IsReleased() const;
		bool IsValid() const;

		unsigned char GetKeyCode() const;

	private:
		EventType type;
		unsigned char key;

	};
	class Keyboard
	{
	public:
		void Create();
		void Destroy();

		void Update();

		bool IsKeyPressed(char key);
		bool IsKeyReleased(char key);
		bool IsKeyDown(char key);
		bool IsKeyUp(char key);

		bool KeyBufferEmpty();
		bool CharBufferEmpty();

		KeyboardEvent ReadKey();
		unsigned char ReadChar();

		void OnKeyPressed(const unsigned char key);
		void OnKeyReleased(const unsigned char key);
		void OnChar(const unsigned char key);

		bool m_keyStates[256];
		std::queue<unsigned char> m_charBuffer;
		std::queue<KeyboardEvent> m_keyBuffer;
		KeyboardEvent m_keyEvent;

	private:
		Keyboard() 
			: m_keyStates()
			, m_charBuffer()
			, m_keyBuffer()
		{ }

	public:
		// Singleton
		static Keyboard& Instance()
		{
			static Keyboard instance;
			return instance;
		}

		Keyboard(Keyboard const&)			= delete;
		void operator=(Keyboard const&)		= delete;

	};
	class Mouse
	{
	public:
		void Create();
		void Destroy();

		union
		{
			struct {
				float x, y;
			};
			float pos[2];
		};

	private:
		Mouse()
			: pos()
		{ }

	public:
		// Singleton
		static Mouse& Instance()
		{
			static Mouse instance;
			return instance;
		}

		Mouse(Mouse const&)				= delete;
		void operator=(Mouse const&)	= delete;
	};
}
