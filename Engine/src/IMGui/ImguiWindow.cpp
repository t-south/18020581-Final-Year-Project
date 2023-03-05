#include "ImguiWindow.h"






//credit to https: //github.com/ocornut/imgui/blob/master/examples/example_glfw_opengl3/main.cpp -- an imgui example
geProject::ImguiWindow::ImguiWindow(GLFWwindow* m_window){
	window = m_window;
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
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
	ImGui::ShowDemoWindow(&show_demo_window);
	ImGui::Text("io.WantCaptureMouse = %d", io.WantCaptureMouse);
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void geProject::ImguiWindow::destroyWindow() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}