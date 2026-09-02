#include <editor/editor.h>
#include <own/stage.h>
#include <editor/gui_misc.h>
#include <editor/gui_st.h>
#include <editor/gui_path.h>

#include <utility>

vector<pair<StageEvent,int>> events = {};

namespace gui_st {
	bool open=false;
	int tabSel = -1;
	void TopMenuBar() {
		TabResultTag topTabResult = TopBar();
	}

	void ContentMenu() {
		if (tabSel == -1)return;

		StageEvent& st = events[tabSel].first;
		int& pt = events[tabSel].second;
		Enemy& enm = st.enm;
		vector<ShooterInstance>& shv=enm.shooters;
		
		if (BeginCombo("Drop", to_string(enm.drops).c_str())) {
			for (size_t i = 0; i < dropList.size(); i++)
			{
				if (Selectable(to_string(i).c_str())) {
					enm.drops =i;
				}
			}
			EndCombo();
		}	

		if (BeginCombo("Path", to_string(pt).c_str())) {
			for (size_t i = 0; i < allPaths.size(); i++)
			{
				if (Selectable(to_string(i).c_str())) {
					pt = i;
				}
			}
			EndCombo();
		}

		// ... inside your ImGui rendering function ...	
		ImGuiSelectionBasicStorage selection_storage;
		for (size_t i = 0; i < shv.size(); i++)
		{
			auto& sh =shv[i];
			selection_storage.SetItemSelected(sh.index,true);
		}
		
		if (BeginCombo("Shooter", "Shooter")) {	
			for (size_t i = 0; i < shooterList.size(); ++i)
			{
				bool is_selected = selection_storage.Contains(i);

				if (ImGui::Selectable(to_string(i).c_str(), &is_selected,ImGuiSelectableFlags_NoAutoClosePopups))
				{
					if (is_selected) shv.push_back(ShooterInstance(i));
					else shv.erase(remove(shv.begin(), shv.end(), ShooterInstance(i)), shv.end());
					
				}
			}
			EndCombo();
		}

	}
}

void OpenStageWindow() { gui_st::open = true; }
using namespace gui_st;
const char* textAspect[] = {
	"TINY_BLUE", "TINY_RED", "TINY_GREEN", "TINY_YELLOW",
	"MAIND_BLUE", "MAID_RED",
	"DRESS_BLUE", "DRESS_RED",
	"SUN_RED",
	"ENEMY_ASPECT_COUNT"
};
void StageWindow() {

	if (!gui_st::open)return;

	StageEvent& st = events[tabSel].first;
	int& pt = events[tabSel].second;
	Enemy& enm = st.enm;

	if (Begin("Stage Editor", &gui_st::open, ImGuiWindowFlags_MenuBar)) {
		gui_st::TopMenuBar();
		TopBar2(events, gui_st::tabSel);
		gui_st::ContentMenu();

		Combo("Aspect", reinterpret_cast<int*>(&enm.aspect), textAspect, IM_ARRAYSIZE(textAspect));
		InputFloat("hp", &enm.hp);
		InputInt("Amout", &st.amount);
		InputFloat("Delay", &st.delayInBetween);
		Text("===Positions===");

		BeginListBox("positions");

		for (size_t i = 0; i < st.positions.size(); i++)
		{
			PushID(i);
			string labelName = "Position" + std::to_string(i);
			InputFloat2(labelName.c_str(), &st.positions[i].x);
			PopID();
		}

		EndListBox();

		End();
	}
	if (IsKeyPressed(ImGuiKey_Z)) {
		EnemySystem::Clear();
		BulletSystem::Clear();
		ItemSystem::Clear();

		ClearEvents();

		float x = static_cast <float>(rand()) / static_cast <float>(RAND_MAX) * 2 - 1;
		float y = static_cast <float>(rand()) / static_cast <float>(RAND_MAX) * 2 - 1;

		//enm.pathQueue = allPaths[pt];
		AddEvent(st);

	}
}