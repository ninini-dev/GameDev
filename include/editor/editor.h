#pragma once
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <iostream>
#include <string>

using namespace std;
using namespace ImGui;

void ImGUILoop();
void ImGuiFrame();
void ImGuiInit(GLFWwindow* window);
void ImGuiShutdown();
extern bool isAnyWindowHovered;