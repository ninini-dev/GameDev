#include <editor/editor.h>
#include <string>

bool open4;

const char* textAspect[] = {
	"TINY_BLUE", "TINY_RED", "TINY_GREEN", "TINY_YELLOW",
	"MAIND_BLUE", "MAID_RED",
	"DRESS_BLUE", "DRESS_RED",
	"SUN_RED",
	"ENEMY_ASPECT_COUNT"
};

#include <vector>
vector<StageEvent> events = { {} };
StageEvent* curEvent=&events.at(0);

void TopTabItemEnm(size_t i) {

	string label = "Path" + std::to_string(i);
	if (BeginTabItem(label.c_str())) {
		EndTabItem();
	}
	if (IsItemClicked()) {
		curEvent = &events[i];
	}
}
void EnmMenu()
{
	Begin("Enemy Data",&open4);
	if (BeginTabBar("tab"))
	{
		for (size_t i = 0; i < events.size(); i++)
			TopTabItemEnm(i);

		if (TabItemButton("-")) {
			events.pop_back();
		}

		if (TabItemButton("+")) {
			events.push_back(StageEvent());
			curEvent = &events.back();
		}

		EndTabBar();

	}

	Combo("Aspect", reinterpret_cast<int*>(&curEvent->enm.aspect), textAspect, IM_ARRAYSIZE(textAspect));
	InputFloat("hp", &curEvent->enm.hp);
	InputInt("Amout", &curEvent->amount);
	InputFloat("Delay", &curEvent->delayInBetween);
	Text("===Positions===");
	
	BeginListBox("positions");
	
	for (size_t i = 0; i < curEvent->positions.size(); i++)
	{
		PushID(i);
		string labelName = "Position" + std::to_string(i);
		InputFloat2(labelName.c_str(), &curEvent->positions[i].x);
		PopID();
	}

	EndListBox();

	if (Button("-") && curEvent->positions.size() > 1)
		curEvent->positions.pop_back();
	SameLine();
	if (Button("+"))
		curEvent->positions.push_back({ 0,0 });

	End();
}