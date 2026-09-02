#include <editor/editor.h>
#include <editor/gui_misc.h>
#include <own/render.h>
void PushColor() {
	PushStyleColor(ImGuiCol_Tab, ImVec4(.6, .4, .08, 1.0f));
	PushStyleColor(ImGuiCol_TabActive, ImVec4(.8, .6, .12, 1.0f));
	PushStyleColor(ImGuiCol_TabHovered, ImVec4(1, .8, .16, 1.0f));
}
void PopColor() {
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
}
ImVec2 iv2(vec2 p) {
	int v[4];
	glGetIntegerv(GL_VIEWPORT, v);
	int w, h;
	glfwGetWindowSize(glfwWindow, &w, &h);
	return ImVec2(v[0] + (p.x + 1) * v[2] / 2, -v[1] + (h - v[3]) + (-p.y + 1) * v[3] / 2);
}