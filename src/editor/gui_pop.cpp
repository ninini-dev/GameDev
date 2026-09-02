#include <editor/editor.h>
#include <editor/gui_pop.h>
#include <editor/gui_drop.h>
#include <editor/gui_enm.h>
#include <editor/gui_sh.h>
#include <own/render.h>
#include <editor/gui_path.h>
#include <editor/gui_st.h>

bool bounds = false;
bool grid = false;
bool ctrlKey = false;
bool click3 = false;

bool testView=false;
void View() {
	if (IsKeyPressed(ImGuiKey_X)) {
		testView = ! testView;
		if (testView)
			glViewport(0, 0, 480, 640);
		else
			glViewport(0, 0, 800, 800);
	}

	auto draw_list = GetBackgroundDrawList();

	if (IsKeyPressed(ImGuiMod_Ctrl))ctrlKey = true;
	if (IsKeyReleased(ImGuiMod_Ctrl))ctrlKey = false;
	if (IsMouseClicked(2))click3 = true;
	if (IsMouseReleased(2))click3 = false;

	int v[4];
	glGetIntegerv(GL_VIEWPORT, v);
	
	if (click3) {
		auto d = GetIO().MouseDelta;
		//glViewport(200, 200, 400, 400);
		glViewport(v[0] + d.x, v[1] - d.y, v[2], v[3]);
	}
	if (ctrlKey) {
		float w = ImGui::GetIO().MouseWheel*10;
		glViewport(v[0]+w, v[1] + w, v[2] - w*2, v[3] - w*2);
	}

	// ImVec2(v[0],v[1]) works, but {v[0],v[1]} dont, this language sucks
	
	int w, h;
	glfwGetWindowSize(glfwWindow,&w, &h);
	if(bounds)draw_list->AddRect(ImVec2(v[0] - 6, h-v[1]+6), ImVec2(v[2]+v[0]+6,h-v[1]-v[3]-6), ImColor(0, 255, 0), 0, ImDrawFlags_None, 12);
	if(grid)
		for (size_t x = 1; x < 10; x++) {
			draw_list->AddLine(ImVec2(0, 80 * x), ImVec2(800, 80 * x), ImColor(60, 60, 60));
			draw_list->AddLine(ImVec2(80 * x, 0), ImVec2(80 * x, 800), ImColor(60, 60, 60));
		}
}

void PopContextWindow() {
	
	View();
	if (BeginPopupContextVoid("popup"))
	{
	//	if (MenuItem("Add Curve"))AddCurve();
		if (BeginMenu("Edit"))
		{
			if (MenuItem("Drop")) OpenDropWindow(); 
			if (MenuItem("Enemy")) OpenEnmWindow();
			if (MenuItem("Shooter")) OpenShWindow();
			if (MenuItem("Path")) OpenPathWindow();
			if (MenuItem("Stage")) OpenStWindow();
			
			
			EndMenu();
		}	
		if (BeginMenu("View"))
		{
			if (MenuItem("Show Bounds")) bounds = !bounds;
			if (MenuItem("Grid View")) grid = !grid;

			EndMenu();
		}

		EndPopup();
	}
}