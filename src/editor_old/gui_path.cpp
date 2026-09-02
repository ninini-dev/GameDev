#include <editor/editor.h>
#include <own/bezier.h>
#include <string>
#include <editor/gui_path.h>
#include <editor/gui_file2.h>

vector<vector<MovePath>> allPaths = { { {{vec2(-.25f),vec2(-.25f),vec2(.25f),vec2(.25f)},Lineal,1} } };
vector<MovePath>* curPath=&allPaths[0];
MovePath* curCurve=&curPath->at(curSeg);

void SetCurCurve(size_t j = 0) {
	curCurve = &curPath->at(j);
	curSeg = j;
}

void TopTabItemPath(size_t i) {

	string label = "Path" + std::to_string(i);
	if (BeginTabItem(label.c_str())) {
		EndTabItem();
	}
	if (IsItemClicked()) {
		curPath = &allPaths[i];
		SetCurCurve(0);
	}
}
void Menu1() {
	if (Begin("New Window", &open1, ImGuiWindowFlags_MenuBar)) {

		if (BeginMenuBar()) {
			if (MenuItem("Save")) SaveVVT(allPaths,"data/path.data");
			if (MenuItem("Load"))LoadVVT(allPaths, "data/path.data");
			EndMenuBar();
		}
		if (BeginTabBar("tab"))
		{
			for (size_t i = 0; i < allPaths.size(); i++)
				TopTabItemPath(i);

			if (TabItemButton("+")) {
				allPaths.push_back({ {{ vec2(-.25f), vec2(-.25f), vec2(.25f), vec2(.25f) }, Lineal, 1 } });
				curPath = &allPaths.back();
				SetCurCurve(0);
			}
			
			EndTabBar();
		}

		Text("===Per Curve Data===");
		Combo("Ease", reinterpret_cast<int*>(&curCurve->ease), textEase, IM_ARRAYSIZE(textEase));
		InputFloat("Duration", &curCurve->duration);
		Text("===Per Path Data===");
		

		End();
	}
}