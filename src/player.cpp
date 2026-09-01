#include <own/player.h>
#include <own/glindex.h>
#include <GLFW/glfw3.h>
#include <immintrin.h>
#include <own/enemy.h>

float pl_x = 0;
float pl_y = -0.9;
bool pl_keys[1024] = {};

namespace Player {
	int count=0;
	float x[MAX_PL] = {};
	float y[MAX_PL] = {};
	int sprite[MAX_PL] = {};

	static const GLfloat u_sprite[128]{
		0,0.8125,0.125,0.1875,
		0,0.25,0.25,0.0625,
	};
}
using namespace Player;

void new_ptama(float offX) {
	if (count+1 >= MAX_PL)return;
	count++;
	x[count]=pl_x+offX;
	y[count] = pl_y;
	sprite[count] = 1;
}
void delete_ptama(int i) {
	x[i] = x[count];
	y[i] = y[count];
	sprite[i] = sprite[count];
	count--;
}
inline void move_ptama(int i, __m256 m_s) {
	__m256 m_y = _mm256_loadu_ps(&y[i]);
	m_y = _mm256_add_ps(m_y,m_s);
	_mm256_storeu_ps(&y[i], m_y);
}//las balas fantasmas pueden causar overflow?

#include <iostream>
inline void map_ptama() {

	__m256 m_o = _mm256_set1_ps(1.5f);
	__m256 m_16 = _mm256_set1_ps(16.0f);
	__m256i m_48 = _mm256_set1_epi32(48);

	for (int i = 0; i <= count; i += 8) {//COLLISION
		__m256 m_x = _mm256_loadu_ps(&x[i]);
		__m256 m_y = _mm256_loadu_ps(&y[i]);

		m_x = _mm256_add_ps(m_x, m_o);
		m_y = _mm256_add_ps(m_y, m_o);

		m_x = _mm256_mul_ps(m_x, m_16);
		m_y = _mm256_mul_ps(m_y, m_16);

		__m256i m_xi = _mm256_cvtps_epi32(_mm256_floor_ps(m_x));
		__m256i m_yi = _mm256_cvtps_epi32(_mm256_floor_ps(m_y));

		__m256i m_code = _mm256_add_epi32(_mm256_mullo_epi32(m_yi, m_48), m_xi);

		alignas(32) int codes[8];
		_mm256_storeu_si256((__m256i*)codes, m_code);


		for (int j = 0; j < 8; ++j) {
			if (i + j > count) return;
			//Enemy::col_map[codes[j]].push_back(i + j);
			auto& size = Enemy::col_map_size[codes[j]];
			Enemy::col_map[codes[j]][size] = i + j;
			size++;
			size = std::min(size, MAX_COL_PER_CELL);
		}

	}

}

void playerLoop() {
	static float step = 1 / 60.0 * .5;
	float col[MAX_BULLET] = {};
	pl_x += (pl_keys[GLFW_KEY_RIGHT] - pl_keys[GLFW_KEY_LEFT]) * step;
	pl_y += (pl_keys[GLFW_KEY_UP] - pl_keys[GLFW_KEY_DOWN]) * step;
	static int t = 4;
	t--;
	if (!t) {
		for (size_t i = 0; i < 10; i++) new_ptama(-.25f+i*.5f*.1);
		t = 4;
	}

	__m256 m_s = _mm256_set1_ps(1.0 / 60.0);
	for (size_t i = 0; i <= count; i+=8)
	{
		move_ptama(i,m_s);
	}
	for (size_t i = 1; i <= count;)
	{
		if (x[i] > 1 || y[i] > 1 || x[i] < -1 || y[i] < -1 || col[i]) {
			//col[i] = col[count - 1];
			delete_ptama(i);
			continue;
		}
		i++;
	}
	map_ptama();
	x[0] = pl_x;
	y[0] = pl_y;
	sprite[0] = 0;

	glUniform4fv(SpriteRender::U_SPR, 32, (const GLfloat*)u_sprite);

	glBindTexture(GL_TEXTURE_2D, SpriteRender::TEX_PL);

	int off = 16 * sizeof(GLfloat);
	int size = (count) * sizeof(GLfloat);
	int offSize= MAX_BULLET * sizeof(GLfloat);
	glBufferSubData(GL_ARRAY_BUFFER, off, size+ sizeof(GLfloat), x);
	off += offSize;
	glBufferSubData(GL_ARRAY_BUFFER, off, size+ sizeof(GLfloat), y);
	float one = 1, zero = 0, sub=-1;
	off += offSize;
	glBufferSubData(GL_ARRAY_BUFFER, off, sizeof(GLfloat), &zero);
	glClearBufferSubData(GL_ARRAY_BUFFER, GL_R32F, off+ sizeof(GLfloat), size, GL_RED, GL_FLOAT, &sub);
	off += offSize;
	glBufferSubData(GL_ARRAY_BUFFER, off, sizeof(GLfloat), &one);
	glClearBufferSubData(GL_ARRAY_BUFFER, GL_R32F, off+ sizeof(GLfloat), size, GL_RED, GL_FLOAT, &zero);
	off += offSize;
	glBufferSubData(GL_ARRAY_BUFFER, off, (count+1) * sizeof(GLint), sprite);

	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, count+1);//IDK why (count+1) fails
}