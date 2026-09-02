#include <editor/editor.h>
#include <editor/gui_pop.h>
#include <editor/gui_drop.h>
#include <editor/gui_enm.h>
#include <editor/gui_sh.h>
#include <editor/gui_path.h>
#include <editor/gui_st.h>
#include <laserTest/laser.h>

bool isAnyWindowHovered = false;

void ImGuiFrame() {

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	NewFrame();

}
void ImGuiInit(GLFWwindow* window) {

	IMGUI_CHECKVERSION();
	CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
}
void ImGuiShutdown() {

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	DestroyContext();
}
void ImGUILoop() {
	/*
	OpenPopup("Manual");
	if (BeginPopupModal("Manual")) {
		Text("Right Click in an empty space to open a context menu\n\
Right Click on a tab to attatch it to a selected enemy\n\
Press Z to test an enemy\n\
Gaspi not allowed");
		EndPopup();
	}*/
	LaserLoop();

	PathWindow();
	DropWindow();
	EnmWindow();
	ShWindow();
	StWindow();
	PopContextWindow();
	isAnyWindowHovered = false;
	Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); 
	
	
}