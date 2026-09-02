#pragma once
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_operations.h>
#include <own/item.h>
#include <own/bezier.h>
#include <own/shooter.h>
#include <own/stage.h>


using namespace ImGui;

void ImGUILoop(); 
void TestEnm();
void ImGuiFrame(); 
void ImGuiInit(GLFWwindow* window); 
void ImGuiShutdown();

void BezierWIndow();
void ContextWindow();
void Menu1();
void BulletLoop();
void DropMenu(); 
void EnmMenu();

void BuildStageFile(Enemy enm);
void ReadStageFile();

void SetActiveTabColors();


//sextern StageEvent testEvent;
//extern ShooterParams shooterList[];
extern bool open1, open2, open3,open4,click1,gridView;