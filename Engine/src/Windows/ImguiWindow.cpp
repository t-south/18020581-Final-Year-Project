#include "ImguiWindow.h"


//credit to https: //github.com/ocornut/imgui/blob/master/examples/example_glfw_opengl3/main.cpp -- an imgui example
geProject::ImguiWindow::ImguiWindow(GLFWwindow* m_window, int windowWidth, int windowHeight, FrameBuffer* fBuffer): windowWidth(windowWidth), windowHeight(windowHeight){
	frameBuffer = fBuffer;
	window = m_window;
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard ;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.IniFilename = "../../../gui_default_layout.ini";	
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 450");
}

geProject::ImguiWindow::~ImguiWindow() {}


void geProject::ImguiWindow::start(int display_w, int display_h, float mouse_x, float mouse_y) {
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2(display_w, display_h);
	io.MousePos = ImVec2(mouse_x, mouse_y);
}


void geProject::ImguiWindow::onEvent() {}

void geProject::ImguiWindow::update(float deltaTime, std::shared_ptr<Scene> scene) {
	bool show_demo_window = true;
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplOpenGL3_NewFrame();
	ImGui::NewFrame();
	ImGui_ImplGlfw_NewFrame();	
	dockWindow();
	scene->updateSceneImgui();	
	//ImGui::ShowDemoWindow(&show_demo_window);
	gameViewWindow();
	//ImGui::Text("io.WantCaptureMouse = %d", io.WantCaptureMouse);
	ImGui::End();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void geProject::ImguiWindow::destroyWindow() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void geProject::ImguiWindow::dockWindow(){
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_MenuBar;
	ImGui::Begin("docker window", (bool*)true, window_flags);	
	ImGui::PopStyleVar(2);	
	ImGui::DockSpace(ImGui::GetID("docker window"));
	
}

void geProject::ImguiWindow::gameViewWindow() {
	ImVec2 vPos = viewPos;
	ImVec2 vSize = viewSize;
	ImGui::BeginMainMenuBar();
	if (ImGui::MenuItem("Play", "", windowRunning, !windowRunning)) {
		eventSystem.publish(new GameStartEvent());
		windowRunning = true;
	}
	if (ImGui::MenuItem("Stop", "", !windowRunning, windowRunning)) {	
		eventSystem.publish(new GameStopEvent());
		windowRunning = false;
	}
	if (ImGui::Button("Save")) {
		eventSystem.publish(new GameSaveEvent());
	}
	if (ImGui::MenuItem("Editor", "", loadLevel, !loadLevel)) {
		eventSystem.publish(new GameLoadEvent(1));
		loadLevel = false;
	}
	if (ImGui::MenuItem("Level 1", "", loadLevel, !loadLevel)) {
		eventSystem.publish(new GameLoadEvent(2));
		loadLevel = false;
	}
	ImGui::EndMainMenuBar();
	ImGui::Begin("gameViewWindow");	
	
	ImVec2 size = getMaxViewPort();
	ImVec2 position = ImGui::GetContentRegionAvail();
	position.x -= ImGui::GetScrollX();
	position.y -= ImGui::GetScrollY();
	position.x = (position.x / 2.0f) - (size.x / 2.0f) + ImGui::GetCursorPosX();
	position.y = (position.y / 2.0f) - (size.y / 2.0f) + ImGui::GetCursorPosY();
	ImGui::Image((ImTextureID)frameBuffer->getTextureId(), size, ImVec2(0, 1), ImVec2(1, 0));
	viewPos = ImGui::GetCursorScreenPos();
	viewPos.x -= ImGui::GetScrollX();
	viewPos.y -= ImGui::GetScrollY();
	viewSize = size;
	
	ImGui::SetCursorPos(position);
	
	ImGui::End();
	
	//std::cout << "ViewPos X: " << viewPos[0] << "ViewPos Y: " <<  viewPos[1] << std::endl;
	//std::cout << " ViewSize X: " << viewSize[0] << " ViewSize Y: " << viewSize[1] << std::endl;
	
	
	if (vPos.x != viewPos.x || vPos.y != viewPos.y || vSize.x != viewSize.x || vSize.y != viewSize.y) {
		eventSystem.publishImmediately(new ViewPortEvent(viewPos.x, viewPos.y, viewSize.x, viewSize.y));
		std::cout << "publishing view port event" << std::endl;
	}
}

ImVec2 geProject::ImguiWindow::getViewPos() { return viewPos; }
ImVec2 geProject::ImguiWindow::getViewSize() { return viewSize; }



ImVec2 geProject::ImguiWindow::getMaxViewPort() {
	float aspectSize = (16.0f / 9.0f);
	ImVec2 windowSize = ImGui::GetContentRegionAvail();
	windowSize.x -= ImGui::GetScrollX();
	windowSize.y -= ImGui::GetScrollY();
	float aspectWidth = windowSize.x;
	float aspectHeight = aspectWidth / aspectSize;
	if (aspectHeight > windowSize.y) {		
		aspectHeight = windowSize.y;
		aspectWidth = aspectHeight * aspectSize;
	}
	return ImVec2(aspectWidth, aspectHeight);
}
