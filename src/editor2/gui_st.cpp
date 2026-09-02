#include <editor/editor.h>
#include <editor/gui_st.h>
#include <vector>
#include <editor/gui_misc.h>
#include <own/stage.h>
#include <editor/gui_enm.h>
namespace gui_st {
	bool open;
	int tabSel = 0;
	void Content();
}
using namespace gui_st;

struct EventRegister {
	StageEvent st;
	int enm; 
};

vector<EventRegister> eventsList = { EventRegister() };
StageEvent& SelEvent() { return eventsList[tabSel].st; }

void UpdateEnemy(size_t enm) {
	eventsList[tabSel].enm= enm;
}
void OpenStageWindow() { open = true; }
void gui_st::Content() {
	InputInt("Amout", &SelEvent().amount);
	InputFloat("Delay", &SelEvent().delayInBetween);

	SeparatorText("Positions");

	BeginListBox("#positions");
	for (size_t i = 0; i < SelEvent().positions.size(); i++)
	{
		PushID(i);
		string labelName = "Position" + std::to_string(i);
		InputFloat2(labelName.c_str(), &SelEvent().positions[i].x);
		PopID();
	}

	EndListBox();

	if (Button("-") && SelEvent().positions.size() > 1)
		SelEvent().positions.pop_back();
	SameLine();
	if (Button("+"))
		SelEvent().positions.push_back({ 0,0 });
}
void StageWindow() {
	if (Begin("Stage Editor", &open, ImGuiWindowFlags_MenuBar)) {

		//TopBar2(eventsList, tabSel);
		if (tabSel != -1)Content();

		End();
	}
	if (IsKeyPressed(ImGuiKey_Z)) {
		EnemySystem::Clear();
		BulletSystem::Clear();
		ItemSystem::Clear();

		ClearEvents();

		//SelEvent().enm = GetEnemy(eventsList[tabSel].enm);
		AddEvent(SelEvent());
	}
}