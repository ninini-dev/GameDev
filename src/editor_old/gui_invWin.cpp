#include <editor/editor.h>
#include <editor/gui_path.h>

ImVec2 cV2(vec2 vec) {
	vec.y *= -1;
	vec += 1;
	vec *= 400;
	auto r = ImVec2(vec.x, vec.y);
	return r;
}
size_t curSeg = 0;
void BezierWIndow() {
	SetNextWindowPos(ImVec2(0.0f, 0.0f));
	SetNextWindowSize(ImVec2(800.0f, 800.0f));
	//ImGui::SetNextWindowBgAlpha(0);
	//ImGui::Begin("New Window2");
	Begin("Invisible Window", NULL, ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground);
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	if (gridView)
		for (size_t x = 1; x < 10; x++) {
			draw_list->AddLine(ImVec2(0, 80 * x), ImVec2(800, 80 * x), ImColor(60, 60, 60));
			draw_list->AddLine(ImVec2(80 * x, 0), ImVec2(80 * x, 800), ImColor(60, 60, 60));
		}
	
	Text("PRESS Z TO TEST");
	//static ImVec2 p0 = { 200,200 }, p1 = { 400,200 }, p2 = { 200,400 }, p3 = { 400,400 };
	//static vector<ImVec2*> list = { &p0,&p1 ,&p2,&p3 };
	//draw_list->AddCircleFilled({200,200}, 10.0f, ImColor(255, 0, 0));
	

	for (size_t j = 0; j < curPath->size(); j++)
	{
		auto& p = curPath->at(j);

		vector<vec2*>list2 = { &p.bezier.p0,&p.bezier.p1 ,&p.bezier.p2,&p.bezier.p3 };

		for (size_t i = 0; i < 4; i++)
		{
			if (j != 0 && i == 0)continue;
			if ((i == 1 || i == 2) && curSeg != j)continue;

			PushID(j * 4 + i); // Unique ID for each point

			ImVec2 d = cV2(*list2[i]);
			draw_list->AddCircleFilled(d, 10.0f, ImColor(255, 0, 0));
			SetCursorScreenPos(d - ImVec2(12, 12)); // Create a small clickable area
			InvisibleButton(("l" + j, "d" + i), ImVec2(25, 25));
			if (IsItemActive())
			{
				SetCurCurve(j);
				
				if (ImGui::IsMouseDragging(0)) {
					auto delta = ImGui::GetIO().MouseDelta / 400;
					*list2[i] += vec2(delta.x, -delta.y); // Update point position directly
					if (i == 2) {
						if (j != curPath->size() - 1)
							curPath->at(curSeg + 1).bezier.p1 =
							p.bezier.p3 + p.bezier.p3 - p.bezier.p2;
					}
					else
						if (i == 1) {
							if (j != 0) {
								curPath->at(curSeg - 1).bezier.p2 =
									p.bezier.p0 + p.bezier.p0 - p.bezier.p1;
							}
						}
				}
				if (ImGui::IsKeyPressed(ImGuiKey_Delete)) {
					curPath->pop_back();
					if (curPath->size() < 1)
						curPath->push_back({ { vec2(-.25f), vec2(-.25f), vec2(.25f), vec2(.25f) }, Lineal, 1 });
					else
						SetCurCurve(curSeg--);
				}
			}
			ImGui::PopID();

			if (j < curPath->size() - 1 && i == 3)curPath->at(j + 1).bezier.p0 = *list2[i];
		}
		//*list2[i] = { d.x / 400,-d.y / 400 };

		draw_list->AddBezierCubic(cV2(*list2[0]), cV2(*list2[1]), cV2(*list2[2]), cV2(*list2[3]),
			curSeg == j ? ImColor(255, 0, 0) : ImColor(0, 0, 255), 3);
	}
	
	draw_list->AddLine(cV2(curCurve->bezier.p0), cV2(curCurve->bezier.p1), IM_COL32_WHITE);
	draw_list->AddLine(cV2(curCurve->bezier.p2), cV2(curCurve->bezier.p3), IM_COL32_WHITE);
	
	ImGui::End();
}