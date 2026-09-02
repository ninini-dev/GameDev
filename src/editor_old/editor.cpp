#include <editor/editor.h>
#include <own/stage.h>
#include <own/bulLet.h>
#include <editor/gui_path.h>
#include <editor/gui_events.h>

//StageEvent testEvent;
bool open1,open2,open3,click1,gridView;

bool newItem (const char* label) {
	if (ImGui::MenuItem(label)) {
		click1 = false;
		return true;
	}
	return false;
}
void SetActiveTabColors() {
	ImGuiStyle& style = ImGui::GetStyle();
	ImGui::PushStyleColor(ImGuiCol_Tab, ImVec4(.75, .8, .75, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TabActive, ImVec4(.75, .8, .75, 1.0f));
}
#include <string>
void ContextWindow() {

	if (IsMouseClicked(1)) {
		click1 = true;
		SetNextWindowPos(GetMousePos());
		SetNextWindowFocus();
	}
	if (click1) {
		Begin("MyItemContextMenu", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse);
		if (!IsWindowFocused())
			click1 = false;
		
		if(newItem("New Curve"))
			curPath->push_back({ { vec2(-.25f), vec2(-.25f), vec2(.25f), vec2(.25f) }, Lineal, 1 });

		if (newItem("Path")) open1 = true;
		if (newItem("Shooter")) open2 = true;
		if (newItem("Drop")) open3 = true;
		if (newItem("Enemy")) open4 = true;
		if (newItem("Grid View"))gridView = !gridView;

		ImGui::End();
	}
}
void ImGUILoop() {

	if (curEvent->positions.size() == 0);	//curEvent->positions = { {0,0} };

	//if (curSeg >= testPaths.size())curSeg = testPaths.size() - 1;
	
	BezierWIndow();
	ContextWindow(); 
	if (open1) Menu1();
	if(open2)BulletLoop();
	if (open3)DropMenu();
	if (open4)EnmMenu();
	;
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
#include <own/item.h>
#include <own/random.h>
void TestEnm() {
	EnemySystem::Clear();
	BulletSystem::Clear();	
	ItemSystem::Clear();

	ClearEvents();
	/*for (size_t i = 0; i < 100; i++)
	{*/
		float x = static_cast <float>(rand()) / static_cast <float>(RAND_MAX) * 2 - 1;
		float y = static_cast <float>(rand()) / static_cast <float>(RAND_MAX) * 2 - 1;
		vector<unsigned int> sh = { 0 };

		//testEvent.enm.curPath.bezier.Displace(vec2(-1, 1));
		curEvent->enm.pathQueue = vector<MovePath>(curPath->rbegin(), curPath->rend());
		AddEvent(*curEvent);

}