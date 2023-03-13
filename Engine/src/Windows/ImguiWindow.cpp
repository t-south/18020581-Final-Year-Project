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

geProject::ImguiWindow::~ImguiWindow() {

}



void geProject::ImguiWindow::start(int display_w, int display_h, float mouse_x, float mouse_y) {
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2(display_w, display_h);
	io.MousePos = ImVec2(mouse_x, mouse_y);
	//glfwGetFramebufferSize(window, &display_w, &display_h);
	//glViewport(0, 0, display_w, display_h);
	//


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
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoNavFocus;
	ImGui::Begin("docker window", (bool*)true, window_flags);	
	ImGui::PopStyleVar(2);
	//ImGui::PopStyleVar();
	ImGui::DockSpace(ImGui::GetID("docker window"));
	
}

void geProject::ImguiWindow::gameViewWindow() {
	ImGui::Begin("gameViewWindow");
	ImVec2 size = ImGui::GetContentRegionAvail();	
	size.x -= ImGui::GetScrollX();
	size.y -= ImGui::GetScrollY();
	if ((size.x / (16.0f / 9.0f)) > size.y) {
		size.x = size.y * (16.0f / 9.0f);
	}
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
	ImGui::End();
	ImGui::SetCursorPos(position);
}

ImVec2 geProject::ImguiWindow::getViewPos() { return viewPos; }
ImVec2 geProject::ImguiWindow::getViewSize() { return viewSize; }