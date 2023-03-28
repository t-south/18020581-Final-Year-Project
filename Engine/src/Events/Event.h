#pragma once

#include <ge_engine/Components.h>

namespace geProject {
	enum Type
	{
		NoType, 
		closeWindow, viewPort, projection, gridToggle, moveWindow,
		deleteEntity, copyEntity, sceneHier,
		keyPressed, keyReleased,
		mousePressed, mouseReleased, mouseMove, mouseScroll,
		gameStart, gameStop, gameSave, gameLoad,
		transForm, spriteRender, rigidBody, boxCollider, circleCollider
	};
	enum Context {
		NoContext = 0,					/* 0b0000000000000000 */
		AppContext = (1 << 0),			/* 0b0000000000000001 */
		EditorContext = (1 << 1),       /* 0b0000000000000010 */
		InputContext = (1 << 2),		/* 0b0000000000000100 */
		KeyboardContext = (1 << 3),		/* 0b0000000000001000 */
		MouseContext = (1 << 4),		/* 0b0000000000010000 */
		MouseButtonContext = (1 << 5),  /* 0b0000000000100000 */
		ImGuiContext = (1 << 6)			/* 0b0000000001000000 */
	};

	class Event {
	protected:		
		typedef unsigned int uInt;
		unsigned int eventContext{ NoContext };
		bool eventHandled{ false };			
		int getContext() { return eventContext; };
		bool isEventHandled(){ return eventHandled; };
		void setHandled(){ eventHandled = true; };
	
	};



	//EDITOR EVENTS

	class GridToggleEvent : public Event {
	public:
		
		GridToggleEvent(bool toggled) : toggled(toggled) { eventContext = EditorContext | AppContext; };
		static int getType() { return Type::gridToggle; };
		int addContext(Context context) { eventContext = getContext() & context; };
		bool contextCheck(Context cat) { return getContext() & cat; }
		bool toggled;
	};

	class CopyEntityEvent : public Event {
	public:
		CopyEntityEvent(unsigned int id): newId(id) { eventContext = EditorContext | AppContext; };
		static int getType() { return Type::copyEntity; };
		int addContext(Context context) { eventContext = getContext() & context; };
		bool contextCheck(Context cat) { return getContext() & cat; }
		unsigned int newId;

	};

	class DeleteEntityEvent : public Event {
	public:
		DeleteEntityEvent(unsigned int id, unsigned int batch, unsigned int index) : entityId(id), renderBatch(batch), renderIndex(index) { eventContext = EditorContext | AppContext; };
		static int getType() { return Type::deleteEntity; };
		int addContext(Context context) { eventContext = getContext() & context; };
		bool contextCheck(Context cat) { return getContext() & cat; }
		unsigned int entityId, renderBatch, renderIndex;
	};

	class HierarchyEvent : public Event {
	public:
		HierarchyEvent(int id, unsigned int mask, std::string name, bool available) : entityId(id), compmask(mask), availability(available), entityName(name) { eventContext = EditorContext | AppContext; };
		static int getType() { return Type::sceneHier; };
		int addContext(Context context) { eventContext = getContext() & context; };
		bool contextCheck(Context cat) { return getContext() & cat; }
		unsigned int compmask;
		int entityId;	
		bool availability;
		std::string entityName;
	};


	//GAME EVENTS

	class GameStartEvent : public Event {
	public:	
		GameStartEvent() { eventContext = AppContext; };
		static int getType() { return Type::gameStart; };	
		int addContext(Context context) { eventContext = getContext() & context; };
		bool contextCheck(Context cat) { return getContext() & cat; }
	};

	class GameStopEvent : public Event {
	public:
		GameStopEvent() { eventContext = AppContext; }
		static int getType() { return Type::gameStop; };	
		int addContext(Context context) { eventContext = getContext() & context; };
		bool contextCheck(Context cat) { return getContext() & cat; }
	};

	class GameSaveEvent : public Event {
	public:
		GameSaveEvent() { eventContext = AppContext; };
		static int getType() { return Type::gameSave; };		
		int addContext(Context context) { eventContext = getContext() & context; };
		bool contextCheck(Context cat) { return getContext() & cat; }
	};

	class GameLoadEvent : public Event {
	public:
		GameLoadEvent(unsigned int id) : sceneId(id){ eventContext = AppContext; };
		static int getType() { return Type::gameLoad; };
		int addContext(Context context) { eventContext = getContext() & context; };
		bool contextCheck(Context cat) { return getContext() & cat; }
		unsigned int sceneId;
	};



	//KEYBOARD EVENTS

	class KeyPressedEvent : public Event {
	public:
		KeyPressedEvent(int kcode, int pressedCount, bool press) : keycode(kcode), pressedCount(pressedCount), pressed(press) { eventContext = KeyboardContext | InputContext | AppContext; };
		static int getType() { return Type::keyPressed; };	
		int addContext(Context context) { eventContext = getContext() & context; };
		inline int getPressedCount() const { return pressedCount; };
		bool contextCheck(Context cat) { return getContext() & cat; }
		int pressedCount, keycode;
		bool pressed;
	};

	//MOUSE EVENTS

	class MouseMoveEvent : public Event {
	public:
		MouseMoveEvent(float posX, float posY) : posX(posX), posY(posY) { eventContext = MouseContext | InputContext | AppContext; };
		float posX, posY;
		static int getType() { return Type::mouseMove; };
		int addContext(Context context) { eventContext = getContext() & context; };
		bool contextCheck(Context cat) {
			return getContext() & cat; 
		}
	};


	class MouseScrollEvent : public Event {
	public:
		MouseScrollEvent(float xOffset, float yOffset, float screenx, float screeny) : xScroll(xOffset), yScroll(yOffset), screenX(screenx), screenY(screeny) { eventContext = MouseContext | InputContext | AppContext; };
		static int getType() { return Type::mouseScroll; };
		int addContext(Context context) { eventContext = getContext() & context; };
		bool contextCheck(Context cat) { return getContext() & cat; }
		float xScroll, yScroll, screenX, screenY;
	};

	class MouseButtonEvent : public Event {
	public:
		MouseButtonEvent(int button, bool mouseDown, float posX, float posY) : mouseButton(button), mouseButtonDown(mouseDown), mouseX(posX), mouseY(posY){ eventContext = MouseContext | InputContext | MouseButtonContext | AppContext; };
		static int getType() { return Type::mousePressed; };	
		bool contextCheck(Context cat) { return getContext() & cat; };
		int addContext(Context context) { eventContext = getContext() & context; };
		int mouseButton;
		bool mouseButtonDown;	
		float mouseX, mouseY;
	};

	//APPLICATION EVENTS

	class TransformEvent : public Event {
	public:
		TransformEvent(int id, float posx, float posy, float rotate) : entityId(id), posX(posx), posY(posy), rotation(rotate) { eventContext = AppContext; };
		static int getType() { return Type::transForm; };		
		bool contextCheck(Context cat) { return getContext() & cat; }
		int addContext(Context context) { eventContext = getContext() & context; };
		int entityId;
		float posX, posY, rotation;
	};

	class SpriteEvent : public Event {
	public:
		SpriteEvent(int id, SpriteRender& spr) : entityId(id), sprite(&spr) { eventContext = AppContext; };
		static int getType() { return Type::spriteRender; };		
		bool contextCheck(Context cat) { return getContext() & cat; }
		int addContext(Context context) { eventContext = getContext() & context; };
		SpriteRender* sprite;
		int entityId;
	};

	class RigidEvent : public Event {
	public:
		RigidEvent(unsigned int id, Rigidbody& rigid) : entityId(id), rigidbody(&rigid) { eventContext = AppContext; };
		static int getType() { return Type::rigidBody; };		
		bool contextCheck(Context cat) { return getContext() & cat; }
		int addContext(Context context) { eventContext = getContext() & context; };
		Rigidbody* rigidbody;
		unsigned int entityId;
	};

	class BoxColliderEvent : public Event {
	public:
		BoxColliderEvent(unsigned int id, float offX, float offY, float sizeX, float sizeY, float origX, float origY) 
			: entityId(id), offSetX(offX), offsetY(offY), boxSizeX(sizeX), boxSizeY(sizeY), originX(origX), originY(origY){	eventContext = AppContext;};
		static int getType() { return Type::boxCollider; };		
		bool contextCheck(Context cat) { return getContext() & cat; }
		int addContext(Context context) { eventContext = getContext() & context; };
		float offSetX, offsetY, boxSizeX, boxSizeY, originX, originY;
		unsigned int entityId;
	};

	class CircleColliderEvent : public Event {
	public:
		CircleColliderEvent(int id, CircleCollider& circle) : entityId(id), circlecollider(&circle) { eventContext = AppContext; };
		static int getType() { return Type::circleCollider; };		
		bool contextCheck(Context cat) { return getContext() & cat; }
		int addContext(Context context) { eventContext = getContext() & context; };
		CircleCollider* circlecollider;
		unsigned int entityId;
	};

	class ProjectionEvent : public Event {
	public:
		ProjectionEvent(glm::mat4 pinv, glm::mat4 vinv) : projInv(pinv), viewInv(vinv) { eventContext = AppContext; };
		static int getType() { return Type::projection; };
		bool contextCheck(Context cat) { return getContext() & cat; }
		int addContext(Context context) { eventContext = getContext() & context; };
		glm::mat4 projInv, viewInv;
	};

	class ViewPortEvent : public Event {
	public:
		ViewPortEvent(float winposX, float winposY, float winsizeX, float winsizeY): windowPosX(winposX), windowPosY(winposY), windowSizeX(winsizeX), windowSizeY(winsizeY){ eventContext = AppContext; };
		static int getType() { return Type::viewPort; };		
		bool contextCheck(Context cat) { return getContext() & cat; }
		int addContext(Context context) { eventContext = getContext() & context; };
		float windowPosX, windowPosY, windowSizeX, windowSizeY;
	};



}