#include <vector>
#include <own/item.h>
#include <editor/editor.h>
#include <editor/gui_drop.h>
#include <editor/gui_enm.h>
#include <editor/gui_misc.h>

namespace gui_drop {
	bool open = false;
	int tabSel=-1;
	int dropSel=-1;
	bool hovered=false;
	vector<size_t> idList;
	size_t id=0;

	size_t Size() { return idList.size(); }

	void Push() {
		dropList.push_back({});
		idList.push_back(id);
		id++;
	}
	void Pop(size_t i){
		dropList.erase(dropList.begin() + i);
		idList.erase(idList.begin() + i);
		if (i < tabSel)tabSel--; else if (i == tabSel)tabSel = -1;
		if (i < dropSel)dropSel--; else if (i == dropSel)dropSel = -1;
	}
	void Tab() {
		if (BeginTabBar("Tab"))
		{
			for (size_t i = 0; i < Size();)
			{
				bool iOpen = true;

				bool color = i == dropSel;
				if (color)PushColor();
				if (BeginTabItem(to_string(i).c_str(), &iOpen)) {
					tabSel=i;
					
					if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Right))
					{
						dropSel = i;
						UpdateDrop(idList[i]);
					}
					EndTabItem();
				}
				if (color)PopColor();

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
	const char* names[] = {
		"POWER_UP", "HP_FRAG", "HP_UP", "BOMB_FRAG", "BOMB_UP", "FULL",
		"POWER", "FAITH"
	};
	void Content() {
		if (tabSel == -1)return;

		vector<Drop>& dv = dropList[tabSel];

		Columns(2, "#DoubleColumn", false);

		for (size_t i = 0; i < dv.size();)
		{
			Drop& d = dv[i];
			PushID(i);
			Combo("type", reinterpret_cast<int*>(&d.item), names, IM_ARRAYSIZE(names));
			InputInt("amount", &d.amount);
			if (InputFloat2("radius", &d.rMin))
				if (d.rMax < d.rMin)d.rMax = d.rMin;
			
			NextColumn();

			if (Button("-")) dv.erase(dv.begin() + i);
			else i++;

			NextColumn();
			Separator();
			PopID();
		}
		Columns(1);
		if (Button("+"))dv.push_back({ POWER,5,0,.1 });
	}
}
using namespace gui_drop;

size_t GetDrop(size_t id) {
	for (size_t i = 0; i < Size(); i++)
		if (idList[i] == id) return i;
	cout << "ERROR: Drop not Found" << endl;
	return -1;
}

void SetDrop(size_t id) {
	dropSel = GetDrop(id);
}

void OpenDropWindow() {
	open = true;
}

void DropWindow() {
	if (!open)return;
	if (Begin("Drop Editor", &open, ImGuiWindowFlags_MenuBar)) {
		Tab();
		Content();
		isAnyWindowHovered += IsWindowHovered();
		End();
	}
	
}