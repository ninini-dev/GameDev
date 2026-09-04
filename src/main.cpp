#include <own/laser.h>
#include <own/shader.h>
#include <iostream>
#include <vector>
#include <chrono>
#include <glm/glm.hpp>
#include <own/render.h>
#include <own/bullet.h>
#include <own/player.h>
#include <own/enemy.h>
#include <own/glindex.h>

using namespace std;
using namespace glm;

bool gameLoop() {
	static auto time_step = 1.0 / 60.0;
	static auto lastTime = chrono::steady_clock::now();

	auto currentTime = chrono::steady_clock::now();
	if (chrono::duration<double>(currentTime - lastTime).count() >= time_step) {

		//std::cout << "\033[H\033[2J" << std::flush;

		GLFrame();

		glUseProgram(SpriteRender::SHAD);
		glBindVertexArray(SpriteRender::VAO);
		glBindBuffer(GL_ARRAY_BUFFER, SpriteRender::VBO);
		bulletLoop();

		playerLoop();
		enemyLoop();
		//laserUpdate();

		lastTime = chrono::steady_clock::now();

		return 1;
	}
	return 0;
}
int main() {

	GLFWStart();

	while (IsWindowOpen()) {

		if (gameLoop()) {
			GLFWFrame();
		}
	}
	GLFWShutdown();

	return 0;
}