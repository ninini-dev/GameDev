#include <editor/editor.h>
#include <editor/gui_misc.h>
#include <own/enemy.h>
#include <bitset>
#include <editor/gui_enm.h>
#include <editor/gui_path.h>
#include <editor/gui_st.h>
namespace gui_enm {
	bool open = false;
	int tabSel = 0;
}
using namespace gui_enm;

struct EnemyEditorRegister {
	Enemy enm;
	uint_fast8_t path;
	bitset<32> shootersSelected;
};

vector<EnemyEditorRegister> enmList = { {} };

EnemyEditorRegister& RegSel() {
	return enmList[tabSel];
}
Enemy& EnmSel() {
	return RegSel().enm;
}
bool TestPath(int i) { 
	return RegSel().path==i;
}
bool TestShooter(size_t i) {
	return RegSel().shootersSelected.test(i);
}

void UpdateDrop(size_t drop) {
	EnmSel().drops = drop;
}
void UpdatePath(int i) {
	RegSel().path = i;
}
bitset<32>& GetShooters() { return RegSel().shootersSelected; }

void UpdateShooters(int i, bool push) {
	if (shooterList.size() <= i)return;

	auto& sh = EnmSel().shooters;

	if (!(push ^ find(sh.begin(), sh.end(), i) == sh.end()))
	{
		sh.push_back(i); 
	}
	else
	{
		sh.erase(sh.begin() + i);
	}
}

void OpenEnmWindow() { gui_enm::open = true; }
const char* textAspect[] = {
	"TINY_BLUE", "TINY_RED", "TINY_GREEN", "TINY_YELLOW",
	"MAIND_BLUE", "MAID_RED",
	"DRESS_BLUE", "DRESS_RED",
	"SUN_RED"
};

void Content() {

	Combo("Aspect", reinterpret_cast<int*>(&EnmSel().aspect), textAspect, IM_ARRAYSIZE(textAspect));
	InputFloat("hp", &EnmSel().hp);

}
#include <own/stage.h>
bool testView=false;
void EnmWindow() {
	if (IsKeyPressed(ImGuiKey_Z)) {
		EnemySystem::Clear();
		BulletSystem::Clear();
		ItemSystem::Clear();

		ClearEvents();
		auto st=StageEvent();
		st.enm = RegSel().enm;
		st.positions.push_back({ 0,0 });
		AddEvent(st);
	}

	if (!gui_enm::open)return;

	if (Begin("Enemy Editor", &gui_enm::open, ImGuiWindowFlags_MenuBar)) {
		//TopMenuBar();
		auto temp=TopBar2(enmList, gui_enm::tabSel);
		if (temp != -1) { UpdateEnemy(temp); cout << temp; }

		if(tabSel!=-1)Content();
		//gui_enm::ContentMenu();

		End();
	}
}