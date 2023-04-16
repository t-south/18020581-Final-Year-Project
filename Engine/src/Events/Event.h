#pragma once
#include <ge_engine/Entity.h>
#include <ge_engine/Components.h>

namespace geProject {
	enum Type
	{
		NoType, 
		closeWindow, viewPort, projection, gridToggle, moveWindow,
		deleteEntity, copyEntity, sceneHier,
		keyPressed, keyReleased,
		mousePressed, mouseReleased, mouseMove, mouseScroll,
		gameStart, gameStop, gameSave, gameLoad, playerFound, attackfound,
		transForm, spriteRender, rigidBody, boxCollider, circleCollider, viewCollider,
		beginContact, endContact, preSolve, postSolve		
	};
	enum Context {
		NoContext = 0,					/* 0b0000000000000000 */
		EditorContext = (1 << 0),       /* 0b0000000000000001 */
		ImGuiContext = (1 << 1),		/* 0b0000000000000010 */
		GameplayContext = (1 << 2),		/* 0b0000000000000100 */
		MenuContext = (1<< 3)
	};

	class Event {
	protected:	
		Event(int context) : eventContext(context) {};
		unsigned int eventContext{ NoContext };
		bool eventHandled{ false };			
		int getContext() { 
			return eventContext; 
		};
		bool isEventHandled(){ return eventHandled; };
		void setHandled(){ eventHandled = true; };
	};

	//EDITOR EVENTS


	class CopyEntityEvent : public Event {
	public:
		CopyEntityEvent(int context, unsigned int id): Event(context), newId(id) {};
		static int getType() { return Type::copyEntity; };
		int addContext(Context context) { eventContext = getContext() & context; };
		bool contextCheck(Context cat) { return getContext() & cat; }
		unsigned int newId;

	};

	class DeleteEntityEvent : public Event {
	public:
		DeleteEntityEvent(int context, unsigned int id, unsigned int batch, unsigned int index) : Event(context), entityId(id), renderBatch(batch), renderIndex(index) {};
		static int getType() { return Type::deleteEntity; };
		int addContext(Context context) { eventContext = getContext() & context; };
		bool contextCheck(Context cat) { return getContext() & cat; }
		unsigned int entityId, renderBatch, renderIndex;
	};

	class HierarchyEvent : public Event {
	public:
		HierarchyEvent(int context, int id, unsigned int mask, std::string name, bool available) : Event(context), entityId(id), compmask(mask), availability(available), entityName(name) {};
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
		GameStartEvent(int context): Event(context){};
		static int getType() { return Type::gameStart; };	
		int addContext(Context context) { eventContext = getContext() & context; };
		bool contextCheck(Context cat) { return getContext() & cat; }
	};

	class GameStopEvent : public Event {
	public:
		GameStopEvent(int context): Event(context) {}
		static int getType() { return Type::gameStop; };	
		int addContext(Context context) { eventContext = getContext() & context; };
		bool contextCheck(Context cat) { return getContext() & cat; }
	};

	class GameSaveEvent : public Event {
	public:
		GameSaveEvent(int context): Event(context) {};
		static int getType() { return Type::gameSave; };		
		int addContext(Context context) { eventContext = getContext() & context; };
		bool contextCheck(Context cat) { return getContext() & cat; }
	};

	class GameLoadEvent : public Event {
	public:
		GameLoadEvent(int context, unsigned int id, bool goap) : Event(context), sceneId(id), aiActive(goap){};
		static int getType() { return Type::gameLoad; };
		int addContext(Context context) { eventContext = getContext() & context; };
		bool contextCheck(Context cat) { 
			return getContext() & cat; 
		}
		unsigned int sceneId;
		bool aiActive;
	};

	class PlayerFoundEvent : public Event {
	public:
		PlayerFoundEvent(int context, unsigned int id, int playerx, int playery, bool found, bool obstruct) : Event(context), enemyId(id), posx(playerx), 
																											  posy(playery), sighted(found), obstructed(obstruct) {};
		static int getType() { return Type::playerFound; };
		int addContext(Context context) { eventContext = getContext() & context; };
		bool contextCheck(Context cat) { return getContext() & cat; }
		float posx;
		float posy;
		bool sighted;
		bool obstructed;
		unsigned int enemyId;

	};

	class AttackSightedEvent : public Event {
	public:
		AttackSightedEvent(int context, unsigned int id, int attackx, int attacky, bool found, bool obstruct) : Event(context), enemyId(id), posx(attackx), 
																												posy(attacky), sighted(found), obstructed(obstruct) {};
		static int getType() { return Type::attackfound; };
		int addContext(Context context) { eventContext = getContext() & context; };
		bool contextCheck(Context cat) { return getContext() & cat; }
		unsigned int enemyId;
		float posx;
		float posy;
		bool sighted;
		bool obstructed;
	};

	//KEYBOARD EVENTS

	class KeyPressedEvent : public Event {
	public:
		KeyPressedEvent(int context, int kcode, int pressedCount) : Event(context), keycode(kcode), pressedCount(pressedCount) {};
		static int getType() { return Type::keyPressed; };	
		int addContext(Context context) { eventContext = getContext() & context; };
		inline int getPressedCount() const { return pressedCount; };
		bool contextCheck(Context cat) { return getContext() & cat; }
		int pressedCount, keycode;	
	};


	class KeyReleasedEvent : public Event {
	public:
		KeyReleasedEvent(int context, int kcode, int pressedCount) : Event(context), keycode(kcode), pressedCount(pressedCount) {};
		static int getType() { return Type::keyReleased; };
		int addContext(Context context) { eventContext = getContext() & context; };
		inline int getPressedCount() const { return pressedCount; };
		bool contextCheck(Context cat) { return getContext() & cat; }
		int pressedCount, keycode;		
	};

	//MOUSE EVENTS

	class MouseMoveEvent : public Event {
	public:
		MouseMoveEvent(int context, float posX, float posY) : Event(context), posX(posX), posY(posY) {};
		float posX, posY;
		static int getType() { return Type::mouseMove; };
		int addContext(Context context) { eventContext = getContext() & context; };
		bool contextCheck(Context cat) {
			return getContext() & cat; 
		}
	};


	class MouseScrollEvent : public Event {
	public:
		MouseScrollEvent(int context, float xOffset, float yOffset, float screenx, float screeny) : Event(context), xScroll(xOffset), yScroll(yOffset), screenX(screenx), screenY(screeny) {};
		static int getType() { return Type::mouseScroll; };
		int addContext(Context context) { eventContext = getContext() & context; };
		bool contextCheck(Context cat) { return getContext() & cat; }
		float xScroll, yScroll, screenX, screenY;
	};

	class MouseButtonEvent : public Event {
	public:
		MouseButtonEvent(int context, int button, bool mouseDown, float posX, float posY) : Event(context), mouseButton(button), mouseButtonDown(mouseDown), mouseX(posX), mouseY(posY){};
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
		TransformEvent(int context, int id, float posx, float posy, float rotate) : Event(context), entityId(id), posX(posx), posY(posy), rotation(rotate) {};
		static int getType() { return Type::transForm; };		
		bool contextCheck(Context cat) { 
			return getContext() & cat; 
		}
		int addContext(Context context) { eventContext = getContext() & context; };
		int entityId;
		float posX, posY, rotation;
	};

	class SpriteEvent : public Event {
	public:
		SpriteEvent(int context, int id, SpriteRender& spr) : Event(context), entityId(id), sprite(&spr) {};
		static int getType() { return Type::spriteRender; };		
		bool contextCheck(Context cat) { return getContext() & cat; }
		int addContext(Context context) { eventContext = getContext() & context; };
		SpriteRender* sprite;
		int entityId;
	};

	class RigidEvent : public Event {
	public:
		RigidEvent(int context, unsigned int id, Rigidbody& rigid) : Event(context), entityId(id), rigidbody(&rigid) {};
		static int getType() { return Type::rigidBody; };		
		bool contextCheck(Context cat) { return getContext() & cat; }
		int addContext(Context context) { eventContext = getContext() & context; };
		Rigidbody* rigidbody;
		unsigned int entityId;
	};

	class BoxColliderEvent : public Event {
	public:
		BoxColliderEvent(int context, unsigned int id, float offX, float offY, float sizeX, float sizeY, float origX, float origY)
			: Event(context), entityId(id), offSetX(offX), offsetY(offY), boxSizeX(sizeX), boxSizeY(sizeY), originX(origX), originY(origY){};
		static int getType() { return Type::boxCollider; };		
		bool contextCheck(Context cat) { return getContext() & cat; }
		int addContext(Context context) { eventContext = getContext() & context; };
		float offSetX, offsetY, boxSizeX, boxSizeY, originX, originY;
		unsigned int entityId;
	};

	class CircleColliderEvent : public Event {
	public:
		CircleColliderEvent(int context, int id, CircleCollider& circle) : Event(context), entityId(id), circlecollider(&circle) {};
		static int getType() { return Type::circleCollider; };		
		bool contextCheck(Context cat) { return getContext() & cat; }
		int addContext(Context context) { eventContext = getContext() & context; };
		CircleCollider* circlecollider;
		unsigned int entityId;
	};

	class ViewColliderEvent : public Event {
	public:
		ViewColliderEvent(int context, int id, ViewCollider& view) : Event(context), entityId(id), viewcollider(&view) {};
		static int getType() { return Type::circleCollider; };
		bool contextCheck(Context cat) { return getContext() & cat; }
		int addContext(Context context) { eventContext = getContext() & context; };
		ViewCollider* viewcollider;
		unsigned int entityId;
	};


	class ProjectionEvent : public Event {
	public:
		ProjectionEvent(int context, glm::mat4 pinv, glm::mat4 vinv) : Event(context), projInv(pinv), viewInv(vinv) {};
		static int getType() { return Type::projection; };
		bool contextCheck(Context cat) { return getContext() & cat; }
		int addContext(Context context) { eventContext = getContext() & context; };
		glm::mat4 projInv, viewInv;
	};

	class ViewPortEvent : public Event {
	public:
		ViewPortEvent(int context, float winposX, float winposY, float winsizeX, float winsizeY): Event(context), windowPosX(winposX), windowPosY(winposY), windowSizeX(winsizeX), windowSizeY(winsizeY){};
		static int getType() { return Type::viewPort; };		
		bool contextCheck(Context cat) { return getContext() & cat; }
		int addContext(Context context) { eventContext = getContext() & context; };
		float windowPosX, windowPosY, windowSizeX, windowSizeY;
	};

	//PHYSICS EVENTS

	class BeginContactEvent : public Event {
	public:
		BeginContactEvent(int context, Entity* entitya, Entity* entityb, bool contact, bool obstruction) : Event(context), entityA(entitya), entityB(entityb), deadly(contact), obstructed(obstruction) {};
		static int getType() { return Type::beginContact; };
		bool contextCheck(Context cat) { return getContext() & cat; }
		int addContext(Context context) { eventContext = getContext() & context; };
		Entity* entityA;
		Entity* entityB;
		bool deadly;
		bool obstructed;
	};

	class EndContactEvent : public Event {
	public:
		EndContactEvent(int context, Entity* entitya, Entity* entityb) : Event(context), entityA(entitya), entityB(entityb) {};
		static int getType() { return Type::endContact; };
		bool contextCheck(Context cat) { return getContext() & cat; }
		int addContext(Context context) { eventContext = getContext() & context; };
		Entity* entityA;
		Entity* entityB;
	};

	class PresolveEvent : public Event {
	public:
		PresolveEvent(int context, Entity* entitya, Entity* entityb) : Event(context), entityA(entitya), entityB(entityb) {};
		static int getType() { return Type::preSolve; };
		bool contextCheck(Context cat) { return getContext() & cat; }
		int addContext(Context context) { eventContext = getContext() & context; };
		Entity* entityA;
		Entity* entityB;
	};

	class PostsolveEvent : public Event {
	public:
		PostsolveEvent(int context, Entity* entitya, Entity* entityb) : Event(context), entityA(entitya), entityB(entityb) {};
		static int getType() { return Type::postSolve; };
		bool contextCheck(Context cat) { return getContext() & cat; }
		int addContext(Context context) { eventContext = getContext() & context; };
		Entity* entityA;
		Entity* entityB;
	};



}