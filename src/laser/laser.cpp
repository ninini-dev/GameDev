#include <laserTest/laser.h>
#include <editor/gui_misc.h>
#include <editor/editor.h>
#include <own/render.h>
#include <own/texture.h>
#include <vector>
#include <own/shader.h>
#include <queue>
#include <glm/gtx/norm.hpp> // For glm::normalize
#include <glm/gtc/matrix_transform.hpp> // For glm::degrees

#include <own/collision.h>
#include <own/bullet.h>


namespace laser{
	struct Laser {
		vec2 pos;
		float speed, angular,angle;
		vec2* points;
		int first, len;
	};
	size_t MAX_LENGTH=100;
	GLuint VBO, VAO, SHAD,TEX;
	Laser lasers[2];

	void LaserLoop() {
		static bool runOne = false;
		if (!runOne) {
			TEX = CreateTexture("res/sprites/etama9.png");
			SHAD = CreateShader(R"(res/shaders/test/test.vert)", R"(res/shaders/test/test.frag)", R"(res/shaders/test/test.geom)");

			glGenVertexArrays(1, &VAO);
			glBindVertexArray(VAO);
			glGenBuffers(1, &VBO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER,5000*sizeof(GLfloat), NULL, GL_DYNAMIC_DRAW);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)(4*sizeof(GLfloat)));
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 1 * sizeof(GLfloat), (void*)(0));
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 1 * sizeof(GLfloat), (void*)(sizeof(GLfloat)));
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 1 * sizeof(GLfloat), (void*)(2*sizeof(GLfloat)));
			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, 1 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

			glVertexAttribDivisor(0,0);
			glVertexAttribDivisor(1,1);
			glVertexAttribDivisor(2, 1);
			glVertexAttribDivisor(3, 1);
			glVertexAttribDivisor(4, 1);

			for (size_t i = 0; i < 2; i++)
			{
				Laser& ins = lasers[i];
				ins = Laser();
				ins.pos = vec2(-.5f+i*.5f);
				ins.speed = 1;
				ins.angular = .05f;
				ins.angle = 0;
				ins.first = 0;
				ins.points = new vec2[MAX_LENGTH];
			}
			
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);

			runOne = true;
		}
			glUseProgram(SHAD);
			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBindTexture(GL_TEXTURE_2D, TEX);


			for (size_t i = 0; i <2; i++)
			{
				Laser& ins = lasers[i];
				ins.angle += ins.angular;
				ins.pos += ins.speed / 60.0f * vec2(sin(ins.angle), cos(ins.angle));

				ins.points[(ins.len + ins.first) % MAX_LENGTH] = ins.pos;

				/*
				if (ins.len != MAX_LENGTH) {

					GLint oldSize;
					glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &oldSize);
					void* oldData = malloc(oldSize);
					glGetBufferSubData(GL_ARRAY_BUFFER, 0, oldSize, oldData);
					glBufferData(GL_ARRAY_BUFFER, oldSize + 6 * sizeof(GLfloat), NULL, GL_DYNAMIC_DRAW);
					glBufferSubData(GL_ARRAY_BUFFER, 0, oldSize, oldData);
					free(oldData);
				}*/
				GLsizei off = 604 * i * sizeof(GLfloat);
				GLfloat a = 2+i, f = (float)ins.first, l = (float)ins.len, w = .02*(i+1);
				glBufferSubData(GL_ARRAY_BUFFER, off, sizeof(GLfloat), &a);
				glBufferSubData(GL_ARRAY_BUFFER, off + sizeof(GLfloat), sizeof(GLfloat), &f);
				glBufferSubData(GL_ARRAY_BUFFER, off + 2 * sizeof(GLfloat), sizeof(GLfloat), &l);
				glBufferSubData(GL_ARRAY_BUFFER, off + 3 * sizeof(GLfloat), sizeof(GLfloat), &w);
				if (ins.len > 1) {
					vec2 data[3] = {
						ins.points[(ins.len + ins.first - 2) % MAX_LENGTH],
						ins.points[(ins.len + ins.first - 1) % MAX_LENGTH],
						ins.points[(ins.len + ins.first) % MAX_LENGTH]
					};
					glBufferSubData(GL_ARRAY_BUFFER, off+4 * sizeof(GLfloat) + (ins.len + ins.first) % MAX_LENGTH * 6 * sizeof(GLfloat), 6 * sizeof(GLfloat), &data[0].x);
			
				}
				glDrawArrays(GL_TRIANGLES, 302, 3 * ins.len);
			}
			//glDrawArraysInstanced(GL_TRIANGLES, 0, 3 * ins.len,2);
			/*GLint first[] = {0,0};
			GLsizei counts[] = { 300,300 };
			GLsizei prim = 1;
			glMultiDrawArrays(GL_TRIANGLES,first,counts,prim);
			*/

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);

			for (size_t i = 0; i < 2; i++)
			{
				Laser& ins = lasers[i];
				if (ins.len < MAX_LENGTH) {
					ins.len++;
				}
				else
				{
					ins.first++;
					ins.first %= MAX_LENGTH;
				}
			}
		
	}
}
void LaserLoop() {
	laser::LaserLoop();
}