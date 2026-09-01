#pragma once
#include <glad/glad.h>
//por seguridad hay que mantener los valores multiplos de 8
#define MAX_BULLET 15000
#define MAX_LASER 1000
#define MAX_ENM 1024
#define VERTEX_PER_LASER 100

namespace LaserRender {
	extern GLuint VBO, VAO, SHAD, TEX, U_LEN, U_OFF;
	extern GLfloat* PTR;
}
namespace SpriteRender {
	extern GLuint VBO, VAO, SHAD, TEX_ETAMA, U_SPR, TEX_ENM,TEX_PL;
}