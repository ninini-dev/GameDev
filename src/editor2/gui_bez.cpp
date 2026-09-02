#include <editor/editor.h>
#include <own/bezier.h>
#include <editor/gui_misc.h>
#include <vector>
#include <editor/gui_path.h>
#include <editor/gui_bez.h>
#include <editor/gui_enm.h>

namespace gui_bez {
	bool open = false;
	int tabSel = -1;
}

using namespace gui_bez;

int cSel = 0;
int pSel = -1;

vector<vector<MovePath>> allPaths = {};

float calc_distance(ImVec2 p1, ImVec2 p2)
{
	const float dx = p2.x - p1.x;
	const float dy = p2.y - p1.y;

	return sqrt((dx * dx) + (dy * dy));
}

ImVec2 iv2(vec2 p) {
	return ImVec2( (p.x + 1) * 400,(-p.y + 1) * 400 );
}

void AddCurve() {
	if (tabSel == -1)return; 
	auto& pathList = allPaths[tabSel];
	
	auto mp= MovePath();

	if (pathList.size() > 0) {
		auto& back = pathList.back().bezier;
		back.p3 = mp.bezier.p0; 
		mp.bezier.p1 = back.p3 + back.p3 - back.p2;
	}

	pathList.push_back(mp);
	cSel = pathList.size() - 1;
}

ImDrawList* draw_list;
vec2* p;

void Circ0(size_t i) {
	if (tabSel == -1)return;
	auto& pathList = allPaths[tabSel];
	ImVec2 d = iv2(*p);
	draw_list->AddCircleFilled(d, 10.0f, ImColor(255, 0, 0));
	if (pSel == i) {
		if (ImGui::IsMouseDragging(0)) {
			auto delta = ImGui::GetIO().MouseDelta;
			p->x += delta.x / 400;
			p->y -= delta.y / 400;
			if (cSel != 0) {
				if (i < 2) {
					if (i == 0) { pathList[cSel - 1].bezier.p3 = *p; p++; }
					pathList[cSel - 1].bezier.p2 = *(p - 1) + *(p - 1) - *p;
					if (i == 0) p--;
				}
			}
			if (cSel != pathList.size() - 1) if(i==2) pathList[cSel + 1].bezier.p1 = *(p + 1) + *(p + 1) - *p;

		}
		if (IsMouseReleased(0))pSel = -1;
	}
	else if (IsMouseClicked(0)) if (calc_distance(GetMousePos(), d) < 12) pSel = i;
	

}
void Circ1(size_t i,size_t j) {
	ImVec2 d = iv2(*p);
	draw_list->AddCircleFilled(d, 10.0f, ImColor(255, 0, 0));
	if (IsMouseClicked(0)) if (calc_distance(GetMousePos(), d) < 12) { cSel = i; pSel = j; }
	d.x -= 4;
	d.y -= 6;
	draw_list->AddText(d, IM_COL32_WHITE, to_string(i).c_str());
}

void BezWindow2() {
	draw_list = GetForegroundDrawList();
	draw_list->AddText({0,0}, IM_COL32_WHITE, "press Z to Test\npress Supr to Delete");

	if (tabSel == -1)return;
	auto& pathList = allPaths[tabSel];
	MovePath& curve = pathList[cSel];

	size_t i = 0;
	p = &curve.bezier.p0;

	draw_list->AddLine(iv2(*p), iv2(*(p + 1)), IM_COL32_WHITE, 1);
	draw_list->AddLine(iv2(*(p + 2)), iv2(*(p + 3)), IM_COL32_WHITE, 1);

	for (i = 0; i < 3; i++)
	{
		Circ0(i);
		p += 1;
	}
	p = &pathList[0].bezier.p0;

	for (i = 0; i < pathList.size(); i++)
	{
		draw_list->AddBezierCubic(iv2(*p), iv2(*(p + 1)), iv2(*(p + 2)), iv2(*(p + 3)), (i == cSel) ? ImColor(255, 0, 0) : ImColor(0, 0, 255), 3);
		Circ1(i, 0);
		p += 5;
	}
	p-=2;
	 Circ0(3); Circ1(i - 1, 3);
}
void OpenBezWindow() { gui_bez::open = true; }

void TopBar3() {
	

	tabSel = -1;

	if (BeginTabBar("tab"))
	{
		for (size_t i = 0; i < allPaths.size();)
		{
			bool iOpen = true;
			bool color = TestPath(i);
			if (color) {
				PushStyleColor(ImGuiCol_Tab, ImVec4(.4, .4, .08, 1.0f));
				PushStyleColor(ImGuiCol_TabActive, ImVec4(.8, .8, .16, 1.0f));
			}
			
			if (BeginTabItem(to_string(i).c_str(), &iOpen)) {
				tabSel = i;	
				if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Right))
				{
					UpdatePath(i);
					cout << "called";
				}
				EndTabItem();
			}	
			if (color) {
				ImGui::PopStyleColor();
				ImGui::PopStyleColor();
			}

			if (IsItemClicked()) { cSel = 0; }
			if (!iOpen) {
				allPaths.erase(allPaths.begin() + i);
				cSel = 0;
				if (tabSel == i)tabSel = -1;
				continue;
			}

			i++;
		}

		if (TabItemButton("+")) 
			allPaths.push_back({ MovePath() });
		

		EndTabBar();
	}
}

const char* textEase[] = { "Lineal",
"EaseInSine", "EaseOutSine", "EaseInOutSine" };
void BezWindow() {
	BezWindow2();
	if (!gui_bez::open)return;

	if (Begin("Path Editor", &gui_bez::open, ImGuiWindowFlags_MenuBar)) {
		
		if (tabSel != -1){
		
			auto& pathList = allPaths[tabSel];

			MovePath& curve = pathList[cSel];
			Combo("Ease", reinterpret_cast<int*>(&curve.ease), textEase, IM_ARRAYSIZE(textEase));
			InputFloat("Duration", &curve.duration);
	
		}
		TopBar3();

		End();
	}
}