#include <editor/editor.h>
#include <editor/gui_path.h>
#include <editor/gui_misc.h>
#include <editor/gui_enm.h>

namespace gui_path{
	int tabSel = -1;
	int pathSel = -1;
	int curSel = -1;
	bool open = false;
	int dotSel = -1;
	vector<vector<MovePath>>  allPaths = {};
	vector<size_t> idList;
	size_t id = 0;

	size_t Size() { return idList.size(); }
	void Push() {
		allPaths.push_back({});
		idList.push_back(id);
		id++;
	}
	void Pop(size_t i) {
		allPaths.erase(allPaths.begin() + i);
		idList.erase(idList.begin() + i);
		if (i < tabSel)tabSel--; else if (i == tabSel)tabSel = -1;
		curSel = -1;
	}

	void Tab() {
		if (BeginTabBar("Tab"))
		{
			for (size_t i = 0; i < Size();)
			{
				bool iOpen = true;

				bool color = i == pathSel;
				if (color)PushColor();
				if (BeginTabItem(to_string(i).c_str(), &iOpen)) {
					tabSel = i;
					if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Right))
					{
						pathSel = i;
						UpdatePath(idList[i]);
						//UpdateDrop(idList[i]);
					}
					EndTabItem();
				}if(IsItemClicked())
					curSel = -1;
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
	const char* names[] = { "Lineal", "EaseInSine", "EaseOutSine", "EaseInOutSine" };
	void Content() {
		if (tabSel == -1)return;

		if (Button("+")) { 
			MovePath path = MovePath();
			if (allPaths[tabSel].size() != 0) {

				path.bezier.p0 = allPaths[tabSel].back().bezier.p3;
				path.bezier.p1 = path.bezier.p0 + path.bezier.p0 - allPaths[tabSel].back().bezier.p2;
				//pathSel = 0;
			}
			allPaths[tabSel].push_back(path); 
		}
		if (curSel == -1)return;

		auto& curve = allPaths[tabSel][curSel];
		Combo("Ease", reinterpret_cast<int*>(&curve.ease), names, IM_ARRAYSIZE(names));
		InputFloat("Duration", &curve.duration);
	}
}
using namespace gui_path;
ImDrawList* draw_list;
bool drag=false; 


float distance(ImVec2 p1, ImVec2 p2)
{
	const float dx = p2.x - p1.x;
	const float dy = p2.y - p1.y;

	return sqrt((dx * dx) + (dy * dy));
} 
#include <string>
void Draw() {
	//draw_list->AddText({100,100}, ImColor(255, 255, 255),to_string(curSel).c_str());
	int v[4];
	glGetIntegerv(GL_VIEWPORT, v);

	if (IsMouseClicked(0) && !isAnyWindowHovered)drag = true;
	else if (IsMouseReleased(0)) { drag = false; dotSel = -1; }

	for (size_t i = 0; i < allPaths[tabSel].size(); i++)
	{
		vec2* p = &allPaths[tabSel][i].bezier.p0;
		draw_list->AddLine(iv2(*p), iv2(*(p + 1)), IM_COL32_WHITE, 1);
		draw_list->AddLine(iv2(*(p + 2)), iv2(*(p + 3)), IM_COL32_WHITE, 1);
		draw_list->AddBezierCubic(iv2(*p), iv2(*(p + 1)), iv2(*(p + 2)), iv2(*(p + 3)), curSel==i?ImColor(255, 0, 0): ImColor(0, 0, 255), 3);

		for (size_t j = 0; j < 3+ (i == allPaths[tabSel].size()-1); j++)
		{
			ImVec2 d = iv2(*p);
			draw_list->AddCircleFilled(d, 10.0f, ImColor(255, 0, 0));
			if (drag) {
				if (dotSel == -1 && IsMouseClicked(0) && distance(GetMousePos(), d) <= 12.5) { 
					dotSel = j + i * 4;
					curSel = i;
				}
				if (IsMouseDragging(0) && dotSel == j + i * 4) {

					auto delta = GetIO().MouseDelta;
					p->x += delta.x /v[2]*2;
					p->y -= delta.y / v[3] * 2;
					//cout << p->x << "/" << p->y << endl;
					if (i != 0) {
						if (j == 0) {
							(p+1)->x += delta.x / v[2] * 2;
							(p+1)->y -= delta.y / v[3] * 2;
							allPaths[tabSel][i - 1].bezier.p2.x += delta.x / v[2] * 2;
							allPaths[tabSel][i - 1].bezier.p2.y -= delta.y / v[3] * 2;
							allPaths[tabSel][i - 1].bezier.p3 = *p;
						}
						if(j==1)allPaths[tabSel][i-1].bezier.p2 = *(p - 1) + *(p - 1) - *p;
					}
					if (i != allPaths[tabSel].size() - 1) {
						if(j==2)allPaths[tabSel][i + 1].bezier.p1 = *(p + 1) + *(p + 1) - *p;
					}
				}
			}
			p++;
		}

	}
}
vector<MovePath> GetPath(size_t id) {
	for (size_t i = 0; i < Size(); i++)
		if (idList[i] == id) return allPaths[i];
	cout << "ERROR: Path not Found" << endl;
	return {};
}
void OpenPathWindow() {
	open = true;
}
void PathWindow() {

	draw_list = GetBackgroundDrawList();
	if(open)
	if (Begin("Path Editor", &open, ImGuiWindowFlags_MenuBar)) {
		Tab();
		Content();
		isAnyWindowHovered += IsWindowHovered();
		End();
	}	
	if (tabSel != -1 && allPaths[tabSel].size() > 0)
		Draw();
}
