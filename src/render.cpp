#include <stb_image.h>
#include <own/render.h>
#include <own/texture.h>
#include <own/shader.h>
#include <vector>
#include <glm/vec4.hpp>
#include <iostream>
using namespace std;

GLuint VAO, VBO, shaderProgram;
GLuint TEX_ENM,TEX_BLT,TEX_PL, TEX_ITEM;

GLFWwindow* glfwWindow; 
int vpW;
int vpH;

const unsigned int MAX_PARTICLES = 100000;

void RenderInitialize() {

	gladLoadGL();

	glViewport(0, 0,800, 800);

	stbi_set_flip_vertically_on_load(true);

	//shaderProgram = CreateShader(R"(res/shaders/default_old/default.vert)", R"(res/shaders/default_old/default.frag)", "");

	shaderProgram = CreateShader(R"(res/shaders/default/default.vert)", R"(res/shaders/default/default.frag)", R"(res/shaders/default/default.geom)");

	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * 5 * sizeof(GLfloat), NULL,GL_DYNAMIC_DRAW );
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(0));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(2*sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(4 * sizeof(GLfloat)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	TEX_ENM = CreateTexture("res/sprites/enemy.png");
	TEX_BLT = CreateTexture("res/sprites/etama.png");
	TEX_PL = CreateTexture("res/sprites/player/pl00/pl00.png");
	TEX_ITEM = CreateTexture("res/sprites/item.png");

	glUseProgram(shaderProgram);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
}
void ChangeTexture(GLuint tex, const GLfloat* aspects, const GLfloat* sizes) {

	glBindTexture(GL_TEXTURE_2D, tex);

	glUniform4fv(glGetUniformLocation(shaderProgram, "aspects"), 9 * 4, aspects);

	glUniform2fv(glGetUniformLocation(shaderProgram, "sizes"), 9 * 2, sizes);

	glUniform1i(glGetUniformLocation(shaderProgram, "tex0"), 0);
}

void GLFrame() {
	glClearColor(.07f, .07f, .07f, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);
}
#include <iostream>
using namespace std;

void sub(int index, GLfloat* data) {
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferSubData(GL_ARRAY_BUFFER, index * 5 * sizeof(GLfloat), 5 * sizeof(GLfloat), data); 
}

void rend(size_t count) {
	/*
	glVertexAttribDivisor(0, 1);
	glVertexAttribDivisor(1, 1);
	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);
	*/
	GLuint indices[] = {0,1,2,3};
	
	glDrawArrays(GL_POINTS, 0, count);
	//glDrawElementsInstanced(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, indices, count);
}
