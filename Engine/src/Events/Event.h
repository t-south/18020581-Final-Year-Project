#pragma once



//contexts of input
//types of input for that context

//convert raw input to context dependent id's

//use callbacks instead of polling, a map of functions will be called depending on context and type

//input every frame
//current active context is evaluated

//go through ordered list of contexts, if input cant be mapped move onto next context
namespace geProject {
	enum Type
	{
		NoType, 
		closeWindow, resizeWindow, focusWindow, stopFocusWindow, moveWindow,
		keyPressed, keyReleased,
		mousePressed, mouseReleased, mouseMove, mouseScroll,
		gameStart, gameStop, gameSave, gameLoad
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
	public:
		
	protected:
		static Event* instance;
		Context eventContext;
		bool eventHandled{ false };
		virtual bool isEventHandled() const = 0;
		virtual void setHandled() = 0;

	};


	//GAME EVENTS

	class GameStartEvent : public Event {
	public:	
		static int getType() { return Type::gameStart; };
		static int getContexts() { return AppCat; };
		virtual bool isEventHandled() const override { return eventHandled; };
		virtual void setHandled() override { eventHandled = true; };
	};

	class GameStopEvent : public Event {
	public:
		static int getType() { return Type::gameStop; };
		static int getContexts() { return AppCat; };
		virtual bool isEventHandled() const override { return eventHandled; };
		virtual void setHandled() override { eventHandled = true; };
	};

	class GameSaveEvent : public Event {
	public:
		static int getType() { return Type::gameSave; };
		static int getContexts() { return AppCat; };
		virtual bool isEventHandled() const override { return eventHandled; };
		virtual void setHandled() override { eventHandled = true; };
	};

	class GameLoadEvent : public Event {
	public:
		GameLoadEvent(unsigned int id) : sceneId(id){};
		unsigned int sceneId;
		static int getType() { return Type::gameLoad; };
		static int getContexts() { return AppCat; };
		virtual bool isEventHandled() const override { return eventHandled; };
		virtual void setHandled() override { eventHandled = true; };
	};



	//KEYBOARD EVENTS

	class KeyEvent : public Event {
	public:
		inline int getKey() const { return keycode; };
	protected:
		int keycode;
		KeyEvent(int keycode) : keycode(keycode) {};
		virtual bool isEventHandled() const = 0;
		virtual void setHandled() = 0;
	};	

	class KeyPressed : public KeyEvent {
	public:
		int pressedCount;
		KeyPressed(int keycode, int pressedCount) : KeyEvent(keycode), pressedCount(pressedCount) {};
		static int getType() { return Type::keyPressed; };
		static int getContexts() { return KeyboardCat | InputCat; };
		inline int getPressedCount() const { return pressedCount; };
		virtual bool isEventHandled() const override { return eventHandled; };
		virtual void setHandled() override { eventHandled = true; };
	};

	class KeyReleased : public KeyEvent {
	public:
		int pressedCount;
		KeyReleased(int keycode, int pressedCount) : KeyEvent(keycode), pressedCount(pressedCount) {};
		static int getType() { return Type::keyReleased; };
		static int getContexts() { return KeyboardCat | InputCat; };
		inline int getPressedCount() const { return pressedCount; };
		virtual bool isEventHandled() const override { return eventHandled; };
		virtual void setHandled() override { eventHandled = true; };
	};


	//MOUSE EVENTS

	class mouseEvent : public Event {
	protected:
		mouseEvent(float posX, float posY) : posX(posX), posY(posY) {};
		float posX, posY;
		virtual bool isEventHandled() const = 0;
		virtual void setHandled() = 0;
	};

	class mousePressed : public mouseEvent {
	public:
		static int getType() { return Type::mousePressed; };
		static int getContexts() { return MouseCat | InputCat | MouseButtonCat; };
		virtual bool isEventHandled() const override { return eventHandled; };
		virtual void setHandled() override { eventHandled = true; };
	};

	class mouseReleased : public mouseEvent {
	public:
		static int getType() { return Type::mouseReleased; };
		static int getContexts() { return MouseCat | InputCat | MouseButtonCat; };
		virtual bool isEventHandled() const override { return eventHandled; };
		virtual void setHandled() override { eventHandled = true; };
	};

	class mouseMove : public mouseEvent {
	public:
		static int getType() { return Type::mouseMove; };
		static int getContexts() { return MouseCat | InputCat; };
		virtual bool isEventHandled() const override { return eventHandled; };
		virtual void setHandled() override { eventHandled = true; };
	};

	class mouseScroll : public mouseEvent {
	public:
		static int getType() { return Type::mouseScroll; };
		static int getContexts() { return MouseCat | InputCat; };
		virtual bool isEventHandled() const override { return eventHandled; };
		virtual void setHandled() override { eventHandled = true; };
	};

}