#pragma once
#include "Event.h"
namespace geProject{
	class mouseEvent : public Event {
	public:
		inline int getKey() const { return keycode };
	protected:
		float posX, posY;
		mouseEvent(float m_posX, float m_posY) : posX(m_posX), posY(m_posY) {};

	};

	class mousePressed : public mouseEvent {
	public:

		mousePressed() : {};
		virtual static Type getType() const override { return Type::mousePressed };
	};

	class mouseReleased : public mouseEvent {
	public:
		mouseReleased() : {};
		virtual static Type getType() const override { return Type::mouseReleased };
	};

	class mouseMove : public mouseEvent {
	public:
		mouseMove() : {};
		virtual static Type getType() const override { return Type::mouseMove };
	};

	class mouseScroll : public mouseEvent {
	public:
		mouseScroll() : {};
		virtual static Type getType() const override { return Type::mouseScroll };
	};

}
