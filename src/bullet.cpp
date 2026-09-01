#include <own/glindex.h>
#include <own/random.h>
#include <own/bullet.h>
#include <glm/vec2.hpp>
#include <glm/trigonometric.hpp>
#include <own/player.h>

#define M_2PI 6.283185307179586

using namespace SpriteRender;

namespace Bullet {
	float x[MAX_BULLET] = { 0 };
	float y[MAX_BULLET] = { 0 };
	float dx[MAX_BULLET] = { 0 };
	float dy[MAX_BULLET] = { 0 };
	float speed[MAX_BULLET] = { 0 };
	int sprite[MAX_BULLET] = { 0 };
	GLuint count = 0;
	static const GLfloat u_sprite[128]{
0,	0.9375	,0.0625	,0.0625,
0,	0.875	,0.0625	,0.0625,
0,	0.8125	,0.0625	,0.0625,
0,	0.75	,0.0625	,0.0625,
0,	0.6875	,0.0625	,0.0625,
0,	0.625	,0.0625	,0.0625,
0,	0.5625	,0.0625	,0.0625,
0,	0.5		,0.0625	,0.0625,
0,	0.4375	,0.0625	,0.0625,
0,	0.375	,0.0625	,0.0625,
0,	0.3125	,0.0625	,0.0625,
0,	0.25	,0.0625	,0.0625,
0,	0.1875	,0.0625	,0.0625,
0,	0.125	,0.0625	,0.0625,
0,	0.0625	,0.0625	,0.0625,
0,	0		,0.0625	,0.0625,


	};
}
using namespace Bullet;

#include <iostream>
void deleteBullet(GLuint id) {
	x[id] = x[count - 1];
	y[id] = y[count - 1];
	dx[id] = dx[count - 1];
	dy[id] = dy[count - 1];
	speed[id] = speed[count - 1];
	
	sprite[id] = sprite[count - 1];
	GLuint off = (16 + MAX_BULLET*4+id) * sizeof(GLfloat);
	glBufferSubData(GL_ARRAY_BUFFER, off, sizeof(GLint), &sprite[count]);

	count--;
	//std::cout << count << std::endl;
}

void newBullet(float nx, float ny) {
	if (count == MAX_BULLET)return;
	static int t=0;
	x[count] = nx;
	y[count] = ny;
	float ang = getRandomRange(0, M_2PI);
	dx[count] = glm::cos(ang);
	dy[count] = glm::sin(ang);
	speed[count] = getRandomRange(.1, 1);
	
	int packedSprite = (t & 0xFFFF) | ((t*2 & 0xFFFF) << 16);//index|offset
	sprite[count] = packedSprite;

	GLuint off = (16+MAX_BULLET*4+count) * sizeof(GLfloat);
	glBufferSubData(GL_ARRAY_BUFFER, off, sizeof(GLint), &sprite[count]);
	t++;
	t %= 5;

	count++;
}
void physics() {
	float col[MAX_BULLET] = {};
	__m256 v_px = _mm256_set1_ps(pl_x);
	__m256 v_py = _mm256_set1_ps(pl_y);
	__m256 v_hitbox = _mm256_set1_ps(0.0625f);
	__m256 v_frame = _mm256_set1_ps(1.0/60.0);

	// Máscara para simular fabsf() (apaga el bit de signo)
	__m256 v_sign_mask = _mm256_castsi256_ps(_mm256_set1_epi32(0x7FFFFFFF));

	__m256 v_one = _mm256_set1_ps(1.0f);
	for (size_t i = 0; i < count; i+=8) {
		//MOVEMENT
		__m256 v_x = _mm256_loadu_ps(&x[i]);
		__m256 v_y = _mm256_loadu_ps(&y[i]);

		__m256 v_dx = _mm256_loadu_ps(&dx[i]);
		__m256 v_dy = _mm256_loadu_ps(&dy[i]);
		
		__m256 v_s = _mm256_loadu_ps(&speed[i]);
		v_s = _mm256_mul_ps(v_s, v_frame);
		
		v_dx = _mm256_mul_ps(v_dx,v_s);
		v_dy = _mm256_mul_ps(v_dy,v_s);

		v_x = _mm256_add_ps(v_x,v_dx);
		v_y = _mm256_add_ps(v_y,v_dy);

		_mm256_storeu_ps(&x[i], v_x);
		_mm256_storeu_ps(&y[i], v_y);

		//COLLISION
		__m256 p_dx = _mm256_sub_ps(v_px, v_x);
		__m256 p_dy = _mm256_sub_ps(v_py, v_y);

		//abs
		__m256 abs_dx = _mm256_and_ps(p_dx, v_sign_mask);
		__m256 abs_dy = _mm256_and_ps(p_dy, v_sign_mask);

		__m256 cmp_x = _mm256_cmp_ps(abs_dx, v_hitbox, _CMP_LT_OQ);
		__m256 cmp_y = _mm256_cmp_ps(abs_dy, v_hitbox, _CMP_LT_OQ);

		__m256 hit_mask = _mm256_and_ps(cmp_x, cmp_y);

		__m256 result = _mm256_and_ps(hit_mask, v_one);

		_mm256_storeu_ps(&col[i], result);
	}

	for (size_t i = 0; i < count; )
	{
		if (x[i] > 1 || y[i] > 1 || x[i] < -1 || y[i] < -1|| col[i]) {
			col[i] = col[count-1];
			deleteBullet(i);
			continue;
		}
		i++;
	}
}
void bulletLoop() {
	//for (size_t i = 0; i < 100; i++) newBullet();
	physics();


	glUniform4fv(U_SPR, 32, (const GLfloat*)u_sprite);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TEX_ETAMA);

	int off = 16 * sizeof(GLfloat);
	glBufferSubData(GL_ARRAY_BUFFER, off, count * sizeof(GLfloat), x);
	off += MAX_BULLET * sizeof(GLfloat);
	glBufferSubData(GL_ARRAY_BUFFER, off, count * sizeof(GLfloat), y);
	off += MAX_BULLET * sizeof(GLfloat);
	glBufferSubData(GL_ARRAY_BUFFER, off, count * sizeof(GLfloat), dx);
	off += MAX_BULLET * sizeof(GLfloat);
	glBufferSubData(GL_ARRAY_BUFFER, off, count * sizeof(GLfloat), dy);
	off += MAX_BULLET * sizeof(GLfloat);
	glBufferSubData(GL_ARRAY_BUFFER, off, count * sizeof(GLint), sprite);
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, count);

	//std::cout << count << std::endl;
}
