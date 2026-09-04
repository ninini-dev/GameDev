#include <own/laser.h>
#include <own/glindex.h>
#include <glm/glm.hpp>
#include <iostream>
#include <own/random.h>

using namespace glm;
using namespace LaserRender;

int lr_count = 0; //cantidad de laseres
float lr_x[MAX_LASER] = { };
float lr_y[MAX_LASER] = { };
float lr_dx[MAX_LASER] = { };
float lr_dy[MAX_LASER] = { };
bool lr_onTurn[MAX_LASER] = { 0 }; // 0 rotando | 1 ya apuntando al jugador
vec2 playerPos = { 0,-0.9 }; //posicion del jugador
GLint lr_off[MAX_LASER] = { 0 }; //offset-puntero de lectura del laser
GLint lr_acum[MAX_LASER] = { 0 }; //acumulado de puntos hasta el momento, utilizado para el offset del UV en el shader
GLsizei lr_len[MAX_LASER] = { 0 }; //longitud m�xima del laser (en puntos)
GLint lr_outPoints[MAX_LASER] = { 0 }; //contador de puntos fuera de la pantalla

GLint aFirst[MAX_LASER * 2] = {}; //inicio draw
GLsizei aCount[MAX_LASER * 2] = {}; //cantidad draw

#include <unordered_map>
std::unordered_multimap<GLsizei, GLsizei> ableLaserMap; //para reciclar lasers, key=longitud, value=id

void deleteLaser(GLsizei id) {
	ableLaserMap.insert({ lr_len[id], id });
	lr_outPoints[id] = -1;
}
int findLaser(size_t len) {
	int id = -1;

	auto it = ableLaserMap.find(len);

	if (it != ableLaserMap.end()) {
		id = it->second;        
		ableLaserMap.erase(it); 
	}
	
	else {
		if (lr_count >= MAX_LASER) return -1;
		id = lr_count;
		lr_len[id] = len;
		lr_count++;
	}

	return id; 
}
void newLaser(vec2 nlp, size_t len) {
	int id = findLaser(len);

	lr_x[id] = nlp.x;
	lr_y[id] = nlp.y;
	lr_dx[id] = 1;
	lr_dy[id] = 0;
	lr_len[id] = len;
	lr_outPoints[id] = 0;
	lr_onTurn[id] = 0;
	lr_off[id] = 0;
}
void shootUpdate() {
	static int timer = 30;
	static size_t lsizes[] = {30,60,100,200};
	if (lr_count < MAX_LASER) { //UN LASER SE VE ESTIRADO Y FINO
		//timer--;
		//if (timer == 0) {
			timer = 30;
			newLaser(vec3(getRandomRange(-1, 1), getRandomRange(-1, 1), 0), lsizes[(int)getRandomRange(0,3.99)]);
		//}
	}
}/*
inline void rotateLaser(int i) {
	static float frame = 1.0f / 60.0f;

	if (!lr_onTurn[i]) {
		float targetAngle = atan2(playerPos.y - lr_y[i], playerPos.x - lr_x[i]);

		float deltaAngle = targetAngle - lr_pos[i].z;
		while (deltaAngle > 3.14159265f) deltaAngle -= 6.2831853f;
		while (deltaAngle < -3.14159265f) deltaAngle += 6.2831853f;

		float maxTurnRate = 2.0f * frame;
		deltaAngle = clamp(deltaAngle, -maxTurnRate, maxTurnRate);

		lr_pos[i].z += deltaAngle;
		if (abs(deltaAngle) < maxTurnRate)lr_onTurn[i] = 1;

	}
}

inline void moveLaser(int i,float iCos, float iSin) {
	static float frame = 1.0f / 60.0f;

	float speed = 0.25;
	float step = speed * frame;

	lr_pos[i].x += step * iCos;
	lr_pos[i].y += step * iSin;
}*/
inline void chOOB(int i) {
	if (lr_x[i] < -1 || lr_y[i] < -1 || lr_x[i] > 1 || lr_y[i] > 1) lr_outPoints[i]++;
	else lr_outPoints[i] = 0;
	if (lr_outPoints[i] >= lr_len[i]) deleteLaser(i);
}
void drawLaser() {

	glUniform1iv(U_OFF, lr_count, (const GLint*)lr_acum);	
	glUniform1iv(U_LEN, lr_count, (const GLint*)lr_len);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TEX);

	//glMultiDrawArrays(GL_TRIANGLE_STRIP, aFirst, aCount, lr_count * 2);

}
inline void moveLaser(int i, __m256 m_x, __m256 m_y, __m256 m_dx, __m256 m_dy, __m256 m_s) {
	m_dx = _mm256_mul_ps(m_dx, m_s);
	m_dy = _mm256_mul_ps(m_dy, m_s);

	m_x = _mm256_add_ps(m_x, m_dx);
	m_y = _mm256_add_ps(m_y, m_dy);

	_mm256_storeu_ps(&lr_x[i], m_x);
	_mm256_storeu_ps(&lr_y[i], m_y);
}
inline void rotateLaser(int i, __m256 m_dx, __m256 m_dy, __m256 m_cos, __m256 m_sin) {
	__m256 m_dx2 = _mm256_sub_ps(_mm256_mul_ps(m_dx,m_cos), _mm256_mul_ps(m_dy,m_sin)); //dxm=dx*cos(t)-dy*sin(t)
	__m256 m_dy2 = _mm256_add_ps(_mm256_mul_ps(m_dx, m_sin), _mm256_mul_ps(m_dy, m_cos)); //dym=dx*sin(t)+dy*cos(t)
	
	_mm256_storeu_ps(&lr_dx[i], m_dx2);
	_mm256_storeu_ps(&lr_dy[i], m_dy2);
}
void laserUpdate() {
	glUseProgram(SHAD);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	shootUpdate();
	static float w = 0.05;
	size_t ao = 0; //acumulado de puntos para el offset del buffer
	//float iCos[MAX_LASER] = {};
	//float iSin[MAX_LASER] = {};


		//MOVEMENT 
		//moveLaser(i,iCos[i],iSin[i]);

	__m256 m_s = _mm256_set1_ps(1.0f / 60.0f * .25f);

	float angle = -0.03f; // Ángulo de rotación por frame
	__m256 m_cos = _mm256_set1_ps(std::cos(angle));
	__m256 m_sin = _mm256_set1_ps(std::sin(angle));

	for (size_t i = 0; i < lr_count; i += 8){
		__m256 m_x = _mm256_loadu_ps(&lr_x[i]);
		__m256 m_y = _mm256_loadu_ps(&lr_y[i]);

		__m256 m_dx = _mm256_loadu_ps(&lr_dx[i]);
		__m256 m_dy = _mm256_loadu_ps(&lr_dy[i]);

		moveLaser(i,m_x,m_y,m_dx,m_dy,m_s);
		rotateLaser(i, m_dx,m_dy,m_cos,m_sin);
	}
	
	//END MOVEMENT

	for (size_t i = 0; i < lr_count; i++)
	{
		if (lr_outPoints[i] == -1) {
			ao += lr_len[i];

			aFirst[i * 2] = 0;
			aFirst[i * 2 + 1] = 0;
			aCount[i * 2] = 0;
			aCount[i * 2 + 1] = 0;
			continue;
		}

		//iCos[i] = cosf(lr_pos[i].z);
		//iSin[i] = sinf(lr_pos[i].z);

		//rotateLaser(i);
		chOOB(i);
		
		GLfloat data[] = {
			(lr_x[i] + lr_dy[i] * w),
			(lr_y[i] - lr_dx[i] * w),
			(lr_x[i] - lr_dy[i] * w),
			(lr_y[i] + lr_dx[i] * w),
		};
		do {
			size_t idx = (lr_off[i] + ao) * 4;

			PTR[idx] = data[0];
			PTR[idx + 1] = data[1];
			PTR[idx + 2] = data[2];
			PTR[idx + 3] = data[3];
			
			lr_off[i]++;
			lr_off[i] %= lr_len[i];
		} while (lr_off[i] == 0); //si es el extremo, copia el valor al incio


		aFirst[i * 2] = (ao + lr_off[i]) * 2;
		aFirst[i * 2 + 1] = (ao) * 2;
		aCount[i * 2] = (lr_len[i] - lr_off[i]) * 2;
		aCount[i * 2 + 1] = lr_off[i] * 2;
		lr_acum[i] = ao + lr_off[i];
		ao += lr_len[i];
	}

	drawLaser();

	//std::cout << lr_count << std::endl;
}
