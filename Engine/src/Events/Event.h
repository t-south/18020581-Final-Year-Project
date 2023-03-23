#pragma once

#include <ge_engine/Components.h>

namespace geProject {
	enum Type
	{
		NoType, 
		closeWindow, viewPort, projection, stopFocusWindow, moveWindow,
		keyPressed, keyReleased,
		mousePressed, mouseReleased, mouseMove, mouseScroll,
		gameStart, gameStop, gameSave, gameLoad,
		transForm, spriteRender, rigidBody, boxCollider, circleCollider
	};
	enum Context {
		NoCategory = 0,				/* 0b0000000000000001 */
		AppCat = (1 << 0),			/* 0b0000000000000010 */
		InputCat = (1 << 1),		/* 0b0000000000000100 */
		KeyboardCat = (1 << 2),		/* 0b0000000000001000 */
		MouseCat = (1 << 3),		/* 0b0000000000010000 */
		MouseButtonCat = (1 << 4)   /* 0b0000000000100000 */
	};

	class Event {
	protected:
		static Event* instance;
		Context eventContext;
		bool eventHandled{ false };			
		bool isEventHandled(){ return eventHandled; };
		void setHandled(){ eventHandled = true; };
	};


	//GAME EVENTS

	class GameStartEvent : public Event {
	public:	
		static int getType() { return Type::gameStart; };
		static int getContexts() { return AppCat; };
		bool contextCheck(Context cat) { return getContexts() & cat; }
	};

	class GameStopEvent : public Event {
	public:
		static int getType() { return Type::gameStop; };
		static int getContexts() { return AppCat; };
		bool contextCheck(Context cat) { return getContexts() & cat; }
	};

	class GameSaveEvent : public Event {
	public:
		static int getType() { return Type::gameSave; };
		static int getContexts() { return AppCat; };
		bool contextCheck(Context cat) { return getContexts() & cat; }
	};

	class GameLoadEvent : public Event {
	public:
		GameLoadEvent(unsigned int id) : sceneId(id){};
		unsigned int sceneId;
		static int getType() { return Type::gameLoad; };
		static int getContexts() { return AppCat; };
		bool contextCheck(Context cat) { return getContexts() & cat; }
	};



	//KEYBOARD EVENTS

	class KeyEvent : public Event {
	public:
		inline int getKey() const { return keycode; };
	protected:
		int keycode;
		KeyEvent(int keycode) : keycode(keycode) {};
		virtual int getContexts() = 0;
	};	

	class KeyPressed : public KeyEvent {
	public:
		int pressedCount;
		KeyPressed(int keycode, int pressedCount) : KeyEvent(keycode), pressedCount(pressedCount) {};
		static int getType() { return Type::keyPressed; };
		static int getContexts() { return KeyboardCat | InputCat; };
		inline int getPressedCount() const { return pressedCount; };
		bool contextCheck(Context cat) { return getContexts() & cat; }
	};

	class KeyReleased : public KeyEvent {
	public:
		int pressedCount;
		KeyReleased(int keycode, int pressedCount) : KeyEvent(keycode), pressedCount(pressedCount) {};
		static int getType() { return Type::keyReleased; };
		static int getContexts() { return KeyboardCat | InputCat; };
		inline int getPressedCount() const { return pressedCount; };
		bool contextCheck(Context cat) { return getContexts() & cat; }
	};


	//MOUSE EVENTS

	class MouseMoveEvent : public Event {
	public:
		MouseMoveEvent(float posX, float posY) : posX(posX), posY(posY) {};
		float posX, posY;
		static int getType() { return Type::mouseMove; };
		static int getContexts() { return MouseCat | InputCat; };
		bool contextCheck(Context cat) { return getContexts() & cat; }
	};


	class MouseScrollEvent : public Event {
	public:
		MouseScrollEvent(float xOffset, float yOffset, float screenx, float screeny) : xScroll(xOffset), yScroll(yOffset), screenX(screenx), screenY(screeny) {};
		static int getType() { return Type::mouseScroll; };
		static int getContexts() { return MouseCat | InputCat; };
		bool contextCheck(Context cat) { return getContexts() & cat; }
		float xScroll, yScroll, screenX, screenY;
	};

	class MouseButtonEvent : public Event {
	public:
		MouseButtonEvent(int button, bool mouseDown, float posX, float posY) : mouseButton(button), mouseButtonDown(mouseDown), mouseX(posX), mouseY(posY){ };
		static int getType() { return Type::mousePressed; };
		static int getContexts() { return MouseCat | InputCat | MouseButtonCat; };	
		bool contextCheck(Context cat) { return getContexts() & cat; }	
		int mouseButton;
		bool mouseButtonDown;	
		float mouseX, mouseY;
	};


	class TransformEvent : public Event {
	public:
		TransformEvent(int id, float posx, float posy, float rotate) : entityId(id), posX(posx), posY(posy), rotation(rotate) {};
		static int getType() { return Type::transForm; };
		static int getContexts() { return AppCat; };
		bool contextCheck(Context cat) { return getContexts() & cat; }		
		int entityId;
		float posX, posY, rotation;
	};

	class SpriteEvent : public Event {
	public:
		SpriteEvent(int id, SpriteRender& spr) : entityId(id), sprite(&spr) {};
		static int getType() { return Type::spriteRender; };
		static int getContexts() { return AppCat; };
		bool contextCheck(Context cat) { return getContexts() & cat; }
		SpriteRender* sprite;
		int entityId;
	};

	class RigidEvent : public Event {
	public:
		RigidEvent(unsigned int id, Rigidbody& rigid) : entityId(id), rigidbody(&rigid) {};
		static int getType() { return Type::rigidBody; };
		static int getContexts() { return AppCat; };
		bool contextCheck(Context cat) { return getContexts() & cat; }
		Rigidbody* rigidbody;
		unsigned int entityId;
	};

	class BoxColliderEvent : public Event {
	public:
		BoxColliderEvent(unsigned int id, float offX, float offY, float sizeX, float sizeY, float origX, float origY) 
			: entityId(id), offSetX(offX), offsetY(offY), boxSizeX(sizeX), boxSizeY(sizeY), originX(origX), originY(origY){};
		static int getType() { return Type::boxCollider; };
		static int getContexts() { return AppCat; };
		bool contextCheck(Context cat) { return getContexts() & cat; }		
		float offSetX, offsetY, boxSizeX, boxSizeY, originX, originY;
		unsigned int entityId;
	};

	class CircleColliderEvent : public Event {
	public:
		CircleColliderEvent(int id, CircleCollider& circle) : entityId(id), circlecollider(&circle) {};
		static int getType() { return Type::circleCollider; };
		static int getContexts() { return AppCat; };
		bool contextCheck(Context cat) { return getContexts() & cat; }
		CircleCollider* circlecollider;
		unsigned int entityId;
	};


	class ViewPortEvent : public Event {
	public:
		ViewPortEvent(float winposX, float winposY, float winsizeX, float winsizeY): windowPosX(winposX), windowPosY(winposY), windowSizeX(winsizeX), windowSizeY(winsizeY){};
		static int getType() { return Type::viewPort; };
		static int getContexts() { return AppCat; };
		bool contextCheck(Context cat) { return getContexts() & cat; }
		float windowPosX, windowPosY, windowSizeX, windowSizeY;
	};

	class ProjectionEvent : public Event {
	public:
		ProjectionEvent(float winposX, float winposY, float winsizeX, float winsizeY) : windowPosX(winposX), windowPosY(winposY), windowSizeX(winsizeX), windowSizeY(winsizeY) {};
		static int getType() { return Type::viewPort; };
		static int getContexts() { return AppCat; };
		bool contextCheck(Context cat) { return getContexts() & cat; }
		float windowPosX, windowPosY, windowSizeX, windowSizeY;
	};


}