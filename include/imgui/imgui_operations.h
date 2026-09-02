#pragma once
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs) {
	return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y);
}
inline ImVec2& operator+=(ImVec2& lhs, const ImVec2& rhs) {
	lhs.x += rhs.x; lhs.y += rhs.y; return lhs;
}
// Sub
inline ImVec2 operator-(const ImVec2& lhs, const ImVec2& rhs) {
	return ImVec2(lhs.x - rhs.x, lhs.y - rhs.y);
}
inline ImVec2& operator-=(ImVec2& lhs, const ImVec2& rhs) {
	lhs.x -= rhs.x; lhs.y -= rhs.y; return lhs;
}
inline ImVec2 operator*(const ImVec2& lhs, const float& rhs) {
	return ImVec2(lhs.x* rhs, lhs.y*rhs);
}inline ImVec2 operator/(const ImVec2& lhs, const float& rhs) {
	return ImVec2(lhs.x / rhs, lhs.y / rhs);
}
inline ImVec2 operator-(const ImVec2& lhs, const float& rhs) {
	return ImVec2(lhs.x / rhs, lhs.y / rhs);
}