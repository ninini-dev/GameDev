#include <bitset>
#include <editor/editor.h>
#include <own/stage.h>
#include <editor/gui_drop.h>
#include <editor/gui_enm.h>
#include <editor/gui_misc.h>
#include <editor/gui_sh.h>
#include <editor/gui_path.h>
#include <editor/gui_st.h>
#include <editor/gui_misc.h>

namespace gui_enm {

	bool open = false;
	int tabSel = -1;
	int enmSel=-1;

	//EnemyRegistry reg = EnemyRegistry();
	vector<EnemyRegistry>regList = {};
	vector<size_t>idList = {};
	size_t id = 0;

	void Push() {
		regList.push_back({});
		idList.push_back(id);
		id++;
	}
	void Pop(size_t i) {
		regList.erase(regList.begin() + i);
		idList.erase(idList.begin() + i);
		if (i < tabSel)tabSel--; else if (i == tabSel)tabSel = -1;
		if (i < enmSel)enmSel--; else if (i == enmSel)enmSel = -1;
	}
	size_t Size() {
		return regList.size();
	}


	const char* names[] = {
	"TINY_BLUE", "TINY_RED", "TINY_GREEN", "TINY_YELLOW", "MAIND_BLUE", 
	"MAID_RED", "DRESS_BLUE", "DRESS_RED", "SUN_RED"
	};

	EnemyRegistry& EnmSel() { return regList[tabSel]; }
	void Content() {
		if (tabSel == -1)return;
		Combo("Aspect", reinterpret_cast<int*>(&EnmSel().aspect), names, IM_ARRAYSIZE(names));
		InputFloat("hp", &EnmSel().hp);

	}
	void Tab() {
		if (BeginTabBar("Tab"))
		{
			for (size_t i = 0; i < Size();)
			{
				bool iOpen = true;

				bool color = i == enmSel;
				if (color)PushColor();
				if (BeginTabItem(to_string(i).c_str(), &iOpen)){
					tabSel = i;
					if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Right))
					{
						enmSel = i;
						UpdateEnemy(idList[i]);
					}
					SetDrop(EnmSel().drop);
					SetShooters(EnmSel().sh);
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
} 

using namespace gui_enm; 
EnemyRegistry GetEnm(size_t id) {
	for (size_t i = 0; i < Size(); i++)
		if (idList[i] == id) return regList[i];
	cout << "ERROR: Enm not Found" << endl;
	return {};
}
void UpdateDrop(size_t drop) { 
	if (tabSel == -1)return;
	EnmSel().drop = drop; 
}
void UpdatePath(size_t path) {
	if (tabSel == -1)return;
	EnmSel().path = path;
}
void UpdateShooters(size_t pos) {
	if (tabSel == -1)return;
	EnmSel().sh.flip(pos);
}
void ShiftShooterSelection(size_t pos) {
	for (size_t i = 0; i < Size(); i++)
		ShiftBitsetAtPos(regList[i].sh, pos);

	ClearStage();
}

void OpenEnmWindow() {
	open = true;
}
void EnmWindow() {
	//TestEnm();
	
	if (!open)return;
	if (Begin("Enemy Editor", &open, ImGuiWindowFlags_MenuBar)) {
		Tab();
		Content(); 
		isAnyWindowHovered += IsWindowHovered();
		End();
	}
}