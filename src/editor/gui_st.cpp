#include <editor/editor.h>
#include <editor/gui_st.h>
#include <own/stage.h>
namespace gui_st {
	bool open = false;
	int tabSel = -1;

	struct StageRegistry {
		StageEvent data;
		size_t enm;
	};
	vector<StageRegistry> events = {};

	StageEvent& SelEvent() {
		return events[tabSel].data;
	}
	void Push() {
		events.push_back({});
	}
	void Pop(size_t i) {
		events.erase(events.begin() + i);
		if (i < tabSel)tabSel--; else if (i == tabSel)tabSel = -1;
	}
	void Tab() {
		if (BeginTabBar("Tab"))
		{
			for (size_t i = 0; i < events.size();)
			{
				bool iOpen = true;

				if (BeginTabItem(to_string(i).c_str(), &iOpen)) {
					tabSel = i;
					//SetDrop(EnmSel().drop);
					//SetShooters(EnmSel().sh);
					EndTabItem();
				}

				if (!iOpen) {
					Pop(i);
					continue;
				}

				i++;
			}
			if (TabItemButton("+")) Push();
			EndTabBar();
		}
	}


	void Content() {
		if (tabSel == -1)return;
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
}
using namespace gui_st;
void OpenStWindow() {
	open = true;
}
#include <editor/gui_drop.h>
#include <editor/gui_path.h>
#include <editor/gui_enm.h>
void TestEnm() {
	if (tabSel == -1)return;
	if (IsKeyPressed(ImGuiKey_Z)) {

		ClearStage();

		auto enm = GetEnm(events[tabSel].enm);

		StageEvent st = SelEvent();
		st.enm = Enemy();
		st.enm.pos = { 0,0 };
		st.enm.drops = GetDrop(enm.drop);
		st.enm.hp = enm.hp;
		st.enm.aspect = enm.aspect;
		st.enm.pathQueue = GetPath(enm.path);
		reverse(st.enm.pathQueue.begin(), st.enm.pathQueue.end());
		//cout << st.enm.pathQueue[0].bezier.p0.x<<" | "<< st.enm.pathQueue[0].bezier.p0.y;
		for (size_t i = 0; i < 32; i++)
		{
			if (enm.sh.test(i)) {
				st.enm.shooters.push_back(i);
				cout << "PUSH SH" << i << endl;
			}
		}

		//SelEvent().enm = GetEnemy(eventsList[tabSel].enm);
		AddEvent(st);
	}
}
void StWindow() {
	if (!open)return;
	if (Begin("Stage Editor", &open, ImGuiWindowFlags_MenuBar)) {
		Tab();
		Content();
		isAnyWindowHovered += IsWindowHovered();
		End();
	}
	TestEnm();
}
void UpdateEnemy(size_t id) {
	if (tabSel == -1)return;
	events[tabSel].enm = id;
}