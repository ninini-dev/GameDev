#include <editor/editor.h>
#include <editor/gui_misc.h>
#include <own/item.h>
#include <editor/gui_drop.h>
#include <editor/gui_enm.h>
namespace gui_drop {
	bool open = false;
	int tabSel = -1;
	int openContext = -1;

	void TopMenuBar() {

		TabResultTag topTabResult = TopBar();

	}
	const char* itemBtAspect[] = {
		"POWER_UP", "HP_FRAG", "HP_UP", "BOMB_FRAG", "BOMB_UP", "FULL",
		"POWER", "FAITH", "ITEM_ASPECT_COUNT"
	};
	void ContentMenu() {
		if (tabSel == -1)return;

		vector<Drop>& dv = dropList[tabSel];
		Columns(2, "doubleColumn", false);
		for (size_t i = 0; i < dv.size();)
		{
			Drop& d = dv[i];
			PushID(i);
			Combo("type", reinterpret_cast<int*>(&d.item), itemBtAspect, IM_ARRAYSIZE(itemBtAspect));
			InputInt("amount", &d.amount);
			if (InputFloat2("radius", &d.rMin)) {
				if (d.rMax < d.rMin)d.rMax = d.rMin;
			}
		
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
bitset<32> selectedColor = {};
void OpenDropWindow() { gui_drop::open = true; }
void DropWindow() {
	if (!gui_drop::open)return;

	if (Begin("Drop Editor", &gui_drop::open, ImGuiWindowFlags_MenuBar)) {
		gui_drop::TopMenuBar();

		auto temp = TopBar2(dropList, gui_drop::tabSel,selectedColor);
		if (temp != -1)openContext=temp;
		if(openContext!=-1) if (BeginPopupContextWindow("popup"))
		{
			if (Selectable("Select")) {
				selectedColor.reset();
				selectedColor.flip(openContext);
				UpdateDrop(openContext);
				openContext = -1;
			}
			EndPopup();
		}

		gui_drop::ContentMenu();

		End();
	}
}