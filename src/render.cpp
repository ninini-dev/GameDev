
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <own/glindex.h>
#include <own/texture.h>
#include <GLFW/glfw3.h>
#include <own/render.h>
#include <own/shader.h>
#include <own/player.h>

GLFWwindow* window;

bool IsWindowOpen() { return !glfwWindowShouldClose(window); }

namespace LaserRender {
	GLuint VBO, VAO, SHAD, TEX,U_LEN,U_OFF;
	GLfloat* PTR;

	void prepareGLProgram() {
		SHAD = CreateShader("def");

		//================VAO===================
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		//================VBO===================
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		auto TOTAL_SIZE_BYTES = MAX_LASER*400 * sizeof(GLfloat);

		GLbitfield flags = GL_MAP_WRITE_BIT |
			GL_MAP_PERSISTENT_BIT |
			GL_MAP_COHERENT_BIT;
		//GLfloat MAX_VERTEX = 100 * 100 * 4;
		glBufferData(GL_ARRAY_BUFFER,TOTAL_SIZE_BYTES, NULL, flags);

		// 1. Asignar la memoria de forma inmutable
		glBufferStorage(GL_ARRAY_BUFFER, TOTAL_SIZE_BYTES, nullptr, flags);

		// 2. Mapear el buffer UNA sola vez y guardar el puntero de por vida
		PTR = (GLfloat*)glMapBufferRange(
			GL_ARRAY_BUFFER, 0, TOTAL_SIZE_BYTES, flags
		);

		//================VAO===================
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)(0));
		
		//================TEXTURE===================
		LaserRender::TEX = CreateTexture("res/sprites/laser1.png");
		glUniform1i(glGetUniformLocation(LaserRender::SHAD, "colorTex0"), 0);
		
		//================UNIFORMS===================
		U_OFF = glGetUniformLocation(SHAD, "uOff");
		U_LEN = glGetUniformLocation(SHAD, "uLen");

		//================UNBIND===================
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
		glBindVertexArray(0);


	}
}

namespace SpriteRender {
	GLuint VBO, VAO, SHAD, TEX_ETAMA, U_SPR, TEX_ENM,TEX_PL;
	void prepareGLProgram() {
		SHAD = CreateShader("sprite");

		//================VAO===================
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		//================VBO===================
		const GLfloat quadVertices[] = {
			-.5,  .5, 0,1,// Arriba-Izquierda
			-.5, -.5, 0,0,// Abajo-Izquierda
			 .5,  .5, 1,1,// Arriba-Derecha
			 .5, -.5  ,1,0// Abajo-Derecha
		};
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		//GLfloat MAX_VERTEX = 100 * 100 * 4;
		glBufferData(GL_ARRAY_BUFFER, (MAX_BULLET*5+16) * sizeof(GLfloat), NULL, GL_DYNAMIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(quadVertices), quadVertices);

		//================VAO===================
		int off = 0;
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)off);
		glEnableVertexAttribArray(0);
		off += sizeof(quadVertices);
		glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(GLfloat), (void*)off);
		glEnableVertexAttribArray(1);
		glVertexAttribDivisor(1, 1);
		off += MAX_BULLET * sizeof(GLfloat);
		glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(GLfloat), (void*)off);
		glEnableVertexAttribArray(2);
		glVertexAttribDivisor(2, 1);
		off += MAX_BULLET * sizeof(GLfloat);
		glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(GLfloat), (void*)off);
		glEnableVertexAttribArray(3);
		glVertexAttribDivisor(3, 1);
		off += MAX_BULLET * sizeof(GLfloat);
		glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(GLfloat), (void*)off);
		glEnableVertexAttribArray(4);
		glVertexAttribDivisor(4, 1);
		off += MAX_BULLET * sizeof(GLfloat);
		glVertexAttribIPointer(5, 1, GL_INT, sizeof(GLint), (void*)off);
		glEnableVertexAttribArray(5);
		glVertexAttribDivisor(5, 1);

		//================UNIFORMS===================


		U_SPR = glGetUniformLocation(SHAD, "uSprite");


		//================TEXTURE===================
		SpriteRender::TEX_ETAMA = CreateTexture("res/sprites/etama.png");
		SpriteRender::TEX_ENM = CreateTexture("res/sprites/enemy.png");
		SpriteRender::TEX_PL = CreateTexture("res/sprites/player/pl00/pl00.png");
		glUniform1i(glGetUniformLocation(SHAD, "colorTex0"), 0);

		//================UNBIND===================
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);


	}
}
void rendInit() {
	//================PREPARE===================
	gladLoadGL();

	glViewport(0, 0, 800, 800);

	SetUpStbiForTextures();

	LaserRender::prepareGLProgram();
	SpriteRender::prepareGLProgram();

	//================FINAL===================
	

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

}
void keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key >= 0 && key < 1024) {
		if (action == GLFW_PRESS) {
			pl_keys[key] = true;
		}
		else if (action == GLFW_RELEASE) {
			pl_keys[key] = false;
		}
	}
}



void GLFWStart() {

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(800, 800, "Game", NULL, NULL);
		
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window,keyCallBack);

	rendInit();
	glfwSwapBuffers(window);
	//IMGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 460");

}
void GLFWShutdown() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
}
void GLFrame() {

	glClearColor(.07f, .17f, .13f, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}
#include <editor/editor.h>
//ImGui::Text("Player Position: (%.2f, %.2f)", pl_x, pl_y);
void GLFWFrame() {
	ImGuiLoop();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	glfwSwapBuffers(window);
	glfwPollEvents();
}