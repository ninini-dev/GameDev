#include <own/enemy.h>
#include <own/glindex.h>
#include <own/random.h>
#include <immintrin.h>
#include <own/player.h>
#include <own/bullet.h>
namespace Enemy {
	float x[MAX_ENM] = {};
	float y[MAX_ENM] = {};
	float benm_x[MAX_ENM] = {};
	float benm_y[MAX_ENM] = {};
	int sprite[MAX_ENM] = {};
	int hp[MAX_ENM] = {};
	int count = 0;
	float b_x[4] = {};
	float b_y[4] = {};
	
	//capaz es mejor usar solamente arrays, si hay muchas balas en una misma celda,
	//puede significar que esta cubierta y puedo ignorar algunas balas.
	//std::vector<int> col_map[ENM_COL_TILES * ENM_COL_TILES];
	int col_map[ENM_COL_TILES * ENM_COL_TILES][MAX_COL_PER_CELL] = {};
	int col_map_size[ENM_COL_TILES * ENM_COL_TILES] = {};
	static const GLfloat u_sprite[128]{
		0,	0.9375,0.09375,0.0625,
		0,	0.75,0.09375,0.0625,
	};
}
using namespace Enemy;
void newEnm() {
	if (count >= MAX_ENM) return;	
	x[count] = getRandomRange(-1, 1);
	y[count] = getRandomRange(-1, 1);
	benm_x[count] = x[count];
	benm_y[count] = y[count];
	hp[count] = 20;
	count++;
}
void delete_enm(GLuint id) {
	x[id] = x[count - 1];
	y[id] = y[count - 1];
	sprite[id] = sprite[count - 1];
	hp[id] = hp[count - 1];
	benm_x[id] = x[count-1];
	benm_y[id] = y[count-1];

	count--;
	//std::cout << count << std::endl;
}

#include <iostream>
inline void anim_enemy() {
	__m256i add_val = _mm256_set1_epi32(0x00010000);
	__m256i mask_val = _mm256_set1_epi32(0x0003FFFF);
	for (int i = 0; i < count; i += 8) {
		//sprite[i] = (sprite[i] + 0x10000) & 0x0003FFFF;	//sprite[i] = (sprite[i] & 0xFFFF) | ((((sprite[i] >> 16) + 1) & 3) << 16);//index|offset
		__m256i m_s = _mm256_loadu_si256((const __m256i*) & sprite[i]);
		m_s = _mm256_add_epi32(m_s, add_val);
		m_s = _mm256_and_si256(m_s, mask_val);
		_mm256_storeu_si256((__m256i*) & sprite[i], m_s);

	}
}
inline void col_enemy(int i, float* col) {
	static const int grid[9] = { -49, -48, -47, -1, 0, 1, 47, 48, 49 };
	static __m256 m_abs = _mm256_castsi256_ps(_mm256_set1_epi32(0x7FFFFFFF));
	static __m256 m_chk = _mm256_set1_ps(0.0625);

	int codeX = (int)((x[i] + 1.5f) * 16.0f);
	int codeY = (int)((y[i] + 1.5f) * 16.0f);
	int codePos = codeX + codeY * 48;

	__m256 e_x = _mm256_set1_ps(x[i]);
	__m256 e_y = _mm256_set1_ps(y[i]);
	for (size_t a = 0; a < 9; a++) {
		int b = codePos + grid[a];
		auto& list = col_map[b];
		for (size_t j = 0; j < col_map_size[b]; j += 8)
		{
			__m256i vindex = _mm256_load_si256((__m256i*) & list[j]);

			__m256 p_x = _mm256_i32gather_ps(Player::x, vindex, 4); // 4 es sizeof(float)
			__m256 p_y = _mm256_i32gather_ps(Player::y, vindex, 4);

			__m256 p_dx = _mm256_sub_ps(e_x, p_x);
			__m256 p_dy = _mm256_sub_ps(e_y, p_y);

			__m256 abs_dx = _mm256_and_ps(p_dx, m_abs);
			__m256 abs_dy = _mm256_and_ps(p_dy, m_abs);

			__m256 cmp_x = _mm256_cmp_ps(abs_dx, m_chk, _CMP_LT_OQ);
			__m256 cmp_y = _mm256_cmp_ps(abs_dy, m_chk, _CMP_LT_OQ);

			__m256 m_hit = _mm256_and_ps(cmp_x, cmp_y);

			int mask = _mm256_movemask_ps(m_hit);

			while (mask > 0) {
				int lane = _tzcnt_u32(mask);  // Obtiene el carril (0..7) que colisionó
				col[list[j + lane]] = 1;      // Marca la bala real
				mask &= (mask - 1);           // Limpia el bit procesado
				hp[i]--;
			}

		}
	}
}
GLuint U_SPR_TRAIL, U_SPR_TIME,U_SPR_VAO, SHAD_TRAIL,VAO_TRAIL;
#include <own/shader.h>
inline int trail_set_VAO() {

	SHAD_TRAIL = CreateShader("trail");

	glGenVertexArrays(1, &VAO_TRAIL);
	glBindVertexArray(VAO_TRAIL);
	//glBindBuffer(GL_ARRAY_BUFFER, SpriteRender::VBO);

	//================VAO===================
	int off = 0;
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)off);
	glEnableVertexAttribArray(0);
	off += sizeof(GLfloat)*16;
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(GLfloat), (void*)off);
	glEnableVertexAttribArray(1);
	glVertexAttribDivisor(1, 6);
	off += MAX_BULLET * sizeof(GLfloat);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(GLfloat), (void*)off);
	glEnableVertexAttribArray(2);
	glVertexAttribDivisor(2, 6);
	/*off += MAX_BULLET * sizeof(GLfloat);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(GLfloat), (void*)off);
	glEnableVertexAttribArray(3);
	glVertexAttribDivisor(3, 6);
	off += MAX_BULLET * sizeof(GLfloat);
	glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(GLfloat), (void*)off);
	glEnableVertexAttribArray(4);
	glVertexAttribDivisor(4, 6);*/
	off += MAX_BULLET * sizeof(GLfloat)*3;
	glVertexAttribIPointer(5, 1, GL_INT, sizeof(GLint), (void*)off);
	glEnableVertexAttribArray(5);
	glVertexAttribDivisor(5, 6);

	//================UNIFORMS===================

	U_SPR_TRAIL = glGetUniformLocation(SHAD_TRAIL, "uSprite");
	U_SPR_TIME = glGetUniformLocation(SHAD_TRAIL, "uTime");
	glUseProgram(SHAD_TRAIL);
	glUniform4fv(U_SPR_TRAIL, 32, (const GLfloat*)u_sprite);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return 0;
}
inline void rend_enemy() {
	static int frame = trail_set_VAO();
	frame++;
	frame %= 16;
	//glUniform4fv(SpriteRender::U_SPR, 32, (const GLfloat*)u_sprite);

	glUseProgram(SHAD_TRAIL);
	glUniform1f(U_SPR_TIME, (float)frame / 4);
	glBindVertexArray(VAO_TRAIL);
	glBindTexture(GL_TEXTURE_2D, SpriteRender::TEX_ENM);

	int off = 16 * sizeof(GLfloat);
	float size = count * sizeof(GLfloat);
	glBufferSubData(GL_ARRAY_BUFFER, off, size, x);
	off += MAX_BULLET * sizeof(GLfloat);
	glBufferSubData(GL_ARRAY_BUFFER, off, size, y);
	//float one = 1, zero = 0;
	off += MAX_BULLET * sizeof(GLfloat)*3;
	//glClearBufferSubData(GL_ARRAY_BUFFER, GL_R32F, off, size, GL_RED, GL_FLOAT, &zero);
	//off += MAX_BULLET * sizeof(GLfloat);
	//glClearBufferSubData(GL_ARRAY_BUFFER, GL_R32F, off, size, GL_RED, GL_FLOAT, &one);
	//off += MAX_BULLET * sizeof(GLfloat);
	glBufferSubData(GL_ARRAY_BUFFER, off, count * sizeof(GLint), sprite);

	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, count*6);
}


float calc_bez(float t,float p[4]) {
	float c = 1 - t;
	return p[0] * c * c * c + p[1] * c * c * t * 3.f + p[2] * c * t * t * 3.f + p[3] * t * t * t;
}
void move_enemy() {
	static float t = 0;
	t += 1.0f / 60.0f;	
	for (int i = count; i >= 0; i--)
	{
		x[i] = benm_x[i] + calc_bez(t, b_x);
		y[i] = benm_y[i] + calc_bez(t, b_y);
		if(abs(x[i])>1.3|| abs(y[i]) > 1.3)delete_enm(i);
	}
	if (t >= 1)t = 0;

}
void enemyLoop() {
	newEnm();
	move_enemy();
	static int frame = 4;
	frame--;
	if (!frame) {
		anim_enemy();
		frame = 4;
	}

	float col[MAX_PL] = {};
	for (int i = count - 1; i >= 0; i--) {
		//newBullet(x[i],y[i]);
		col_enemy(i,col);
		//_mm256_storeu_epi32(&dmg,m_dmg);
		if (hp[i] <= 0) delete_enm(i);
	
	}
	memset(col_map_size, 0, sizeof(col_map_size));

	for (size_t i = Player::count; i > 0 ; i--)
		if(col[i])delete_ptama(i);

	rend_enemy();
	//for (size_t i = 0; i < ENM_COL_TILES * ENM_COL_TILES; i++) Enemy::col_map[i].clear();

}