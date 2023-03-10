#include "ImguiWindow.h"






//credit to https: //github.com/ocornut/imgui/blob/master/examples/example_glfw_opengl3/main.cpp -- an imgui example
geProject::ImguiWindow::ImguiWindow(GLFWwindow* m_window){
	window = m_window;
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfig;
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
	scene->updateSceneImgui();
	//ImGui::ShowDemoWindow(&show_demo_window);
	//ImGui::Text("io.WantCaptureMouse = %d", io.WantCaptureMouse);
	ImGui::End;
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void geProject::ImguiWindow::destroyWindow() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void geProject::ImguiWindow::dockWindow(int width, int height){
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;


	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(width, height));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, ImVec2(0.0f, 0.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, ImVec2(0.0f, 0.0f));
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoNavFocus;
	ImGui::Begin("docker window", (bool*)true, window_flags);
	ImGui::PopStyleVar(2);

}