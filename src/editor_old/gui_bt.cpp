#include <editor/editor.h>
#include <own/shooter.h>
#include <editor/gui_file2.h>
#include <editor/gui_events.h>

const char* textBtAspect[] = {
	"LEAF", "EMPTY_CIRCLE", "CIRCLE", "CAPSULE", "KUNAI", "OFUDA", "BULLET", "B_CAPSULE",
	"START", "GLOW", "RING", "B_TINY", "TINY", "FLASH", "BULLET_ASPECT_COUNT"
};
const char* textBtColor[] = {
	"BLACK", "D_RED", "RED", "D_MAGENTA", "MAGENTA", "D_BLUE", "BLUE", "D_CYAN", "CYAN",
	"D_GREEN", "GREEN", "LIME", "D_YELLOW", "YELLOW", "GOLD", "WHITE", "BULLET_COLOR_COUNT"
};
void SetCustomTabColors() {
	ImGuiStyle& style = ImGui::GetStyle();
	ImGui::PushStyleColor(ImGuiCol_Tab, ImVec4(.75, .8, .75, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TabActive, ImVec4(.75, .8, .75, 1.0f));
}

vector<ShooterParams> shooterList = {
	{Nico::Bullet(LEAF,RED,false,vec2(0,0),.1,0),1,1,.1,0,0,.1}
};
size_t curSh = 0;
vector<ShooterInstance>* sh() {return &curEvent->enm.shooters;}
int HasShooter(size_t i) {

	for (size_t j = 0; j < sh()->size(); j++)
		if (sh()->at(j).index == i) return j;
	
	return -1;
}

#include <string>
void TopTabItemSh(size_t i) {
	bool test=false;
	if(HasShooter(i)>=0){
		SetCustomTabColors();
		test = true;
	}
	string label = "Sh" + std::to_string(i);
	if (ImGui::BeginTabItem(label.c_str()))
		EndTabItem();

	if (IsItemClicked()) {
		curSh = i;
		auto pos = HasShooter(i);
		if(pos!=-1)
			sh()->erase(sh()->begin() + pos);
		else
			sh()->push_back({ i,0,0 });
		
	}

	if (test) {
		ImGui::PopStyleColor(); ImGui::PopStyleColor();
	}
}

void BulletLoop() {

	ImGui::Begin("Shooter Params", &open2, ImGuiWindowFlags_MenuBar);

	if (BeginMenuBar()) {
		if (MenuItem("Save")) SaveVT(shooterList, "data/sh.data");
		if (MenuItem("Load")) { LoadVT(shooterList, "data/sh.data"); curSh = 0; }
		EndMenuBar();
	}

	auto& shT = shooterList[curSh];
	auto& btT = shT.bullet;
	if (ImGui::BeginTabBar("tab"))
	{
		for (size_t i = 0; i < shooterList.size(); i++)
			TopTabItemSh(i);

		if (TabItemButton("-")) {
			if (shooterList.size() > 1) {
				auto pos = HasShooter(shooterList.size() - 1);
				if (pos != -1) {
					cout << "YEEEES FUCK:" << pos << endl;
					sh()->erase(sh()->begin() + pos);
					TestEnm();
					curSh--;
				}
				shooterList.pop_back();
			}
			else
				shT = {};
		}
		if (TabItemButton("+"))
			shooterList.push_back({ });

		EndTabBar();
	}

	ImGui::Text("===Shooter===");
	ImGui::InputInt("Bullets Amount", reinterpret_cast<int*>(&shT.aB));
	ImGui::InputInt("Arrays Amount", reinterpret_cast<int*>(&shT.aA));
	ImGui::InputFloat("Delay Between Bullet", &shT.dBtwB);
	ImGui::InputFloat("Delay At Begin", &shT.dAtB);
	ImGui::InputFloat("Spread Between Array", &shT.sBtwA);
	ImGui::InputFloat("Rate", &shT.rate);
	ImGui::Text("===Bullet===");
	ImGui::Combo("aspect", reinterpret_cast<int*>(&btT.aspect), textBtAspect, IM_ARRAYSIZE(textBtAspect));
	ImGui::Combo("color", reinterpret_cast<int*>(&btT.color), textBtColor, IM_ARRAYSIZE(textBtColor));
	ImGui::InputFloat("speed", &btT.speed);
	ImGui::End();
}