#pragma once
#include "Event.h"
namespace geProject {
	class KeyEvent : public Event {
	public:
		inline int getKey() const { return keycode };
	protected:
		int keycode;
		KeyEvent(int m_keycode) : keycode(m_keycode) {};

	};

	class KeyPressed : public KeyEvent {
	public:
		int pressedCount;
		KeyPressed(int m_keycode, int m_pressedCount) : KeyEvent(keycode), pressedCount(m_pressedCount) {};
		inline int getPressedCount() const { return pressedCount };
		virtual static Type getType() const override { return Type::keyPressed };
	};

	class KeyReleased : public KeyEvent {
	public:
		int pressedCount;
		KeyReleased(int m_keycode, int m_pressedCount) : KeyEvent(keycode), pressedCount(m_pressedCount) {};
		inline int getPressedCount() const { return pressedCount };
		virtual static Type getType() const override { return Type::keyReleased };
	};
}