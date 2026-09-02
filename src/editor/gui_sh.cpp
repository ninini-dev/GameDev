#include <editor/editor.h>
#include <own/shooter.h>
#include <editor/gui_sh.h>
#include <editor/gui_misc.h>
#include <editor/gui_enm.h>
namespace gui_sh {
	int tabSel = -1;
	bool open=false;
	bitset<32> selection;
	vector<size_t> idList;
	size_t id = 0;

	size_t Size() {
		return shooterList.size();
	}
	void Push() {
		shooterList.push_back({});
		idList.push_back(id);
		id++;
	}
	void Pop(size_t i) {
		shooterList.erase(shooterList.begin() + i);
		idList.erase(idList.begin() + i);
		if (i < tabSel)tabSel--; else if (i == tabSel)tabSel = -1;
		
		ShiftBitsetAtPos(selection,i);
		ShiftShooterSelection(i);
	}
	void Tab() {
		if (BeginTabBar("Tab"))
		{
			for (size_t i = 0; i < Size();)
			{
				bool iOpen = true;

				bool color = selection.test(i);
				if (color) PushColor();
				if (BeginTabItem(to_string(i).c_str(), &iOpen)) {
					tabSel = i;

					if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Right))
					{
						selection.flip(i);
						UpdateShooters(i);
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
	const char* aspectNames[] = {
		"LEAF", "EMPTY_CIRCLE", "CIRCLE", "CAPSULE", "KUNAI", "OFUDA", "BULLET", "B_CAPSULE",
		"START", "GLOW", "RING", "B_TINY", "TINY", "FLASH"
	};
	const char* colorNames[] = {
		"BLACK", "D_RED", "RED", "D_MAGENTA", "MAGENTA", "D_BLUE", "BLUE", "D_CYAN", "CYAN",
		"D_GREEN", "GREEN", "LIME", "D_YELLOW", "YELLOW", "GOLD", "WHITE"
	};
	void Content() {
		if (tabSel == -1)return;

		ShooterParams& sh = shooterList[tabSel];
		Nico::Bullet& bt = sh.bullet;

		SeparatorText("Shooter");
		InputInt("Bullets Amount", reinterpret_cast<int*>(&sh.aB));
		InputInt("Arrays Amount", reinterpret_cast<int*>(&sh.aA));
		InputFloat("Delay Between Bullet", &sh.dBtwB);
		InputFloat("Delay At Begin", &sh.dAtB);
		InputFloat("Spread Between Array", &sh.sBtwA);
		InputFloat("Rate", &sh.rate);
		SeparatorText("Bullet");
		Combo("Aspect", reinterpret_cast<int*>(&bt.aspect), aspectNames, IM_ARRAYSIZE(aspectNames));
		Combo("Color", reinterpret_cast<int*>(&bt.color), colorNames, IM_ARRAYSIZE(colorNames));
		InputFloat("Speed", &bt.speed);
	}
}
using namespace gui_sh;
void SetShooters(bitset<32> setSel) {
	selection = setSel;
}

void OpenShWindow() {
	open = true;
}
void ShWindow() {
	if (!open)return;
	if (Begin("Shooter Editor", &open, ImGuiWindowFlags_MenuBar)) {
		Tab(); 
		Content();
		isAnyWindowHovered += IsWindowHovered();
		End();
	}
}