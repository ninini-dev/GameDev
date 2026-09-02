#pragma once
#include <glad/glad.h>
#include<GLFW/glfw3.h>
void rend(size_t tCount);
void sub(int index, GLfloat* data);
void GLFrame();
void RenderInitialize();
void ChangeTexture(GLuint tex, const GLfloat* aspects, const GLfloat* sizes);
extern GLFWwindow* glfwWindow;