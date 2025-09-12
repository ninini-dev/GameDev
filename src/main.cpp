#include <glad/glad.h>
#include<GLFW/glfw3.h>
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

GLFWwindow* window;

void GLFWFrame() {
	glfwSwapBuffers(window);
	glfwPollEvents();
}

#include <own/bullet.h>
void Shoot() {
	static EnemyAspectTag aspect = TINY_BLUE;
	static int enemiesCount = 0;
	
	//enm.sprite = enemyAspectDictionary[aspect];
	enemiesCount += 1000;
	for (size_t i = 0; i < 1000; i++)
	{
		float x = static_cast <float>(rand()) / static_cast <float>(RAND_MAX)*2-1;
		float y = static_cast <float>(rand()) / static_cast <float>(RAND_MAX)*2-1;
		vector<unsigned int> sh = { 0 };
		EnemySystem::Add(1, Enemy(vec2(x, y), aspect,sh));
		/*BulletSystem::Add(2, Bullet(aspectB, color, vec2(x, y), .1, angle));
		aspectB = (BulletAspect)(((int)aspectB + 1) % ((int)BULLET_ASPECT_COUNT - 1));
		angle += .1;
		color = (BulletColorTag)(((int)color + 1) % ((int)BULLET_COLOR_COUNT));*/
		//EnemySystem::Add(1, Enemy( vec2(x, y), aspect));
		//aspect = (EnemyAspectTag)(((int)aspect + 1) % ((int)ENEMY_ASPECT_COUNT - 1));
	}
}
#include <own/pl.h>
bool simulate() {
	static auto time_step = 1.0f / 60.0f;
	static auto lastTime = chrono::steady_clock::now();
	
	auto currentTime = chrono::steady_clock::now();

	bool result = false;
	if ((chrono::duration<double>(currentTime - lastTime).count() >= time_step)) {

		lastTime = chrono::steady_clock::now();
		GLFrame();
		EnemySystem::Update(time_step);
		BulletSystem::Update(time_step);
		PlLoop();
		GLFWFrame(); 
		result = true;
	}
	return result;
}
void keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods) {
	
	if (key == GLFW_KEY_Z && action == GLFW_PRESS) { 
		
		
		Shoot();
	}
}
int main() {
	EnemySystem::Create();
	BulletSystem::Create();
	glfwInit();
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);	
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);


	window=glfwCreateWindow(800,800,"Game",NULL,NULL);
	
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window,keyCallBack);

	RenderInitialize();

	glfwSwapBuffers(window);

	//AudioTest();
	while (!glfwWindowShouldClose(window)) {
		if (!simulate())continue;
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}