#pragma once
#include <bitset>
#include <glm/vec2.hpp>
#include <editor/editor.h>
using namespace glm;
using namespace ImGui;

void PushColor();
void PopColor(); 

template <size_t Size>
void ShiftBitsetAtPos(bitset<Size>& selection, size_t i) {

	for (; i < Size - 2; i++)
	{
		if (selection.test(i + 1)) selection.set(i);
		else selection.reset(i);
	}
	selection.reset(Size - 1);
} 
ImVec2 iv2(vec2 p);