#pragma once
#include <vector>
#include <editor/editor.h>
#include <bitset>
using namespace std;
typedef enum TabResultTag {
	None, Save, Load
};
TabResultTag TopBar();
template <typename T>
int TopBar2(vector<T>& v, int& tabSel, bitset<32> colorSet = {}) {

	int result=-1;
	tabSel = -1;
	if (BeginTabBar("tab"))
	{
		for (size_t i = 0; i < v.size();)
		{
			bool iOpen = true;

			if (colorSet.test(i)) {
				PushStyleColor(ImGuiCol_Tab, ImVec4(.4, .4, .08, 1.0f));
				PushStyleColor(ImGuiCol_TabActive, ImVec4(.8, .8, .16, 1.0f));
			}
			if (BeginTabItem(to_string(i).c_str(), &iOpen)) {
				tabSel = i;
				if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Right))
				{
					result = i;
				}
				EndTabItem();
			}    
			if (colorSet.test(i)) {
				ImGui::PopStyleColor();
				ImGui::PopStyleColor();
			}

			if (!iOpen) { 
				v.erase(v.begin() + i); 
				if (tabSel == i)tabSel=-1;
				continue; 
			}

			i++;
		}

		if (TabItemButton("+")) 
			v.push_back(T());
		
		EndTabBar();
	}
	return result;
}
