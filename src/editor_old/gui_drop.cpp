#include <editor/editor.h>
#include <own/item.h>
#include <editor/gui_drop.h>
#include <editor/gui_file2.h>
#include <editor/gui_events.h>

const char* itemBtAspect[] = {
	"POWER_UP", "HP_FRAG", "HP_UP", "BOMB_FRAG", "BOMB_UP", "FULL",
	"POWER", "FAITH", "ITEM_ASPECT_COUNT"
};
vector<vector<Drop>> dropsList = {
	{{POWER,5,0,.1f},{FAITH,15,0,.15f}},
	{{POWER,5,0,.1f},{FAITH,15,0,.15f}}
}; 

vector<Drop>* dT() { return curEvent->enm.drops; }
#include <string>
void TopTabItemDrop(size_t i) {

	string label = "Drop" + std::to_string(i);
	bool color = false;
	if (dT() == &dropsList[i]) { SetActiveTabColors(); color = true; }
	if (BeginTabItem(label.c_str())) {
		if (IsKeyPressed(ImGuiKey_Delete)) {
			dropsList.erase(dropsList.begin()+i);
		}
		EndTabItem();
	}
	if (IsItemClicked()) {
		curEvent->enm.drops = &dropsList[i];
	}
	if (color == true) {
		PopStyleColor();
		PopStyleColor();
	}
}
void DropMenu() {
	//curEvent->enm.drops = dT;
	Begin("Drop Params", &open3, ImGuiWindowFlags_MenuBar);
	
	if (BeginMenuBar()) {
		if (MenuItem("Save")) SaveVVT(dropsList, "data/drop.data"); 
		if (MenuItem("Load")) LoadVVT(dropsList, "data/drop.data"); 
		EndMenuBar();
	}
	if (BeginTabBar("tab"))
	{
		for (size_t i = 0; i < dropsList.size(); i++)
			TopTabItemDrop(i);
	
		if (TabItemButton("+")) {
			dropsList.push_back({});
			curEvent->enm.drops = &dropsList.back();
		}
		EndTabBar();
	} 

	for (size_t i = 0; i < dT()->size(); i++)
	{
		PushID(i);
		InputInt("amount", &dT()->at(i).amount);
		InputFloat("rMin", &dT()->at(i).rMin);
		InputFloat("rMax", &dT()->at(i).rMax);
		Combo("type", reinterpret_cast<int*>(&dT()->at(i).item), itemBtAspect, IM_ARRAYSIZE(itemBtAspect));
		PopID();
		Separator();
	}
	if (Button("-") && dT()->size() > 0)
		dT()->pop_back();
	SameLine();
	if (Button("+"))
		dT()->push_back({ POWER,5,0,.1 });


	End();
}