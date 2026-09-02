//#include <glad/glad.h>
//#include<GLFW/glfw3.h>
#include<glm/vec2.hpp>

#include <stb_image.h>

#include <fmod.hpp>

#include <own/shader.h>
#include <own/texture.h>
#include <own/sprite.h>
#include <own/render.h>
#include <own/enemy.h>

#include <iostream>
#include <vector>
#include <chrono>
//#include <own/editor.h>


using namespace std;
using namespace glm;

/*void AudioTest() {

	FMOD_RESULT result;
	FMOD::System* pSystem = nullptr;

	result = FMOD::System_Create(&pSystem);

	pSystem->init(32, FMOD_INIT_NORMAL, nullptr);

	FMOD::Sound* pSound = nullptr;
	pSystem->createSound(R"(C:\Users\W10\Desktop\extlibs\wavs\se_cardget.wav)", FMOD_DEFAULT, nullptr, &pSound);

	FMOD::Channel* pChannel = nullptr;
	result = pSystem->playSound(pSound, nullptr, false, &pChannel);

	if (result == FMOD_OK) {
		cout << "isPlaying" << endl;
	}

	bool bIsPlaying = true;

	/*while (bIsPlaying) {
		pChannel->isPlaying(&bIsPlaying);
		pSystem->update();
	}
}
*/

//GLFWwindow* window;

#include <editor/editor.h>
void GLFWFrame() {
	glfwSwapBuffers(glfwWindow);
	glfwPollEvents();
}

//operator glm::vec2() const { return glm::vec2(x, y); }

#include <own/bullet.h>
#include <own/item.h>
//up,down,right,left,shift
bool keysPressed[] = { false, false, false, false,false };
#include <own/pl.h>
#include <vector>
#include <own/stage.h>
bool simulate() {
	static auto time_step = 1.0f / 60.0f;
	static auto lastTime = chrono::steady_clock::now();
	
	auto currentTime = chrono::steady_clock::now();

	bool result = false;
	if ((chrono::duration<double>(currentTime - lastTime).count() >= time_step)) {

		lastTime = chrono::steady_clock::now();

		ImGuiFrame();
		GLFrame();
		EnemySystem::Update(time_step);
		BulletSystem::Update(time_step);
		ItemSystem::Update(time_step);
		float dy = (keysPressed[0] - keysPressed[1])*time_step;
		float dx = (keysPressed[2] - keysPressed[3])*time_step;
		vec2 dv = vec2(dx, dy);
		if (keysPressed[4])dv *= .5f;
		plPos += dv;
		StageEventLoop(time_step);
		PlLoop();

		ImGUILoop();
		GLFWFrame();

		result = true;
	}
	return result;
}
void keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods) {

	switch (key)
	{
	case GLFW_KEY_UP:
		keysPressed[0] = action > 0;
		break;
	case GLFW_KEY_DOWN:
		keysPressed[1] = action > 0;
		break;
	case GLFW_KEY_RIGHT:
		keysPressed[2] = action > 0;
		break;
	case GLFW_KEY_LEFT:
		keysPressed[3] = action > 0;
		break;
	case GLFW_KEY_LEFT_SHIFT:
		keysPressed[4] = action > 0;
		break;
	default:
		break;
	}
}
int main() {

	EnemySystem::Create();
	BulletSystem::Create();
	ItemSystem::Create();

	glfwInit();
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);	
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);


	glfwWindow =glfwCreateWindow(800,800,"Game",NULL,NULL);
	
	glfwMakeContextCurrent(glfwWindow);

	glfwSetKeyCallback(glfwWindow,keyCallBack);

	RenderInitialize();

	glfwSwapBuffers(glfwWindow);

	//IMGUI
	ImGuiInit(glfwWindow);

	//AudioTest();
	while (!glfwWindowShouldClose(glfwWindow)) {
	
		//ImGui::EndFrame();

		if (!simulate())continue;

		//ImGui


	}
	ImGuiShutdown();

	glfwDestroyWindow(glfwWindow);
	glfwTerminate();
	return 0;
}