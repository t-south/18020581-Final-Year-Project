#pragma once
#ifdef GE_PLATFORM_WINDOWS

extern geProject::Application* geProject::CreateApplication();

int main(int argc, char** argv) {
	geProject::Logging::Init();
	GE_ENGINE_WARN("Initialised log!");
	int a = 5;
	GE_GAME_INFO("test! Var={0}", a);
	auto game = geProject::CreateApplication();
	game->Startup();
	delete game;
	
}

#endif 
