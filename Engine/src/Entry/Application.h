#pragma once

#include <stdio.h>
#include <iostream>
#include <glad/glad.h>
#include "../Windows/ImguiWindow.h"
#include "../Windows/Window.h"
#include "../Render/FrameBuffer.h"
#include "../SceneManager/SceneStates.h"
#include "../SceneManager/LevelEditorScene.h"
#include "../SceneManager/LevelScene.h"
#include "../Clock/Clock.h"
#include <ge_engine/Core.h>

namespace geProject {
	class Application
	{
	public:
		Application();
		virtual ~Application();
		void Startup();
		void loop();
	private:
		SceneStates* sceneManager;
		Window* gameWindow;
		Clock* gameClock;
		ImguiWindow* imguiWindow;
		FrameBuffer* frameBuffer;
		FrameBuffer* selectionTextures;
		void startGamePlay(GameStartEvent* start);
		void stopGamePlay(GameStopEvent* stop);
		void loadGame(GameLoadEvent* load);

	};

	Application* CreateApplication();


}