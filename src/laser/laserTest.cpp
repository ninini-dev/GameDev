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

unordered_multimap<int, unsigned int> laserMap = {};

struct Laser {
	BulletColorTag color;
	size_t len, first,maxLen;
	vec2 pos;
	float speed;
	float angle;
	float agSpeed;
	float width;
	vec2* points;
	vec2 side;
	unordered_multimap<int, unsigned int>::iterator* mapPtrs;
	void Step(size_t index, float delta);
	void Render(size_t i); 
	size_t I(size_t i);
	vec2 P(size_t i);

};

namespace laser {
	GLuint LASER_TEX, shaderProgram, VAO, VBO;
	void RenderInit() {
		LASER_TEX = CreateTexture("res/sprites/etama9.png");
		shaderProgram = CreateShader(R"(res/shaders/laser/laser.vert)", R"(res/shaders/laser/laser.frag)","");
		
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER,0, NULL, GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)(0));

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	void RenderBind() {
		glUseProgram(shaderProgram);
		glBlendFunc(GL_ONE, GL_ONE);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBindTexture(GL_TEXTURE_2D, LASER_TEX);
	}
	void RenderUnBind() {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
}
using namespace laser;

//vec2 P(size_t i) { return points._Get_container()[i]; }
size_t Laser::I(size_t i) { return (first + i) % maxLen; }
vec2 Laser::P(size_t i) {
	return points[I(i)];
}
vec2 T(vec2 a, vec2 b) {
	vec2 dir = b - a;
	dir=normalize(dir);
	dir = vec2(dir.y, -dir.x);
	return dir;
}
//vector<vec2> renderTest;
vec2 p;

void Laser::Render(size_t index) {
	glUniform1f(glGetUniformLocation(shaderProgram, "vertexCount"), len);
	glUniform1f(glGetUniformLocation(shaderProgram, "color"), color);
	glUniform1f(glGetUniformLocation(shaderProgram, "first"), first);
	glDrawArrays(GL_TRIANGLE_STRIP, index * 400+ first * 3, 1 + 3 * bool(first) + (len - first - 1) * 3);
	glUniform1f(glGetUniformLocation(shaderProgram, "first"), first - 1);
	glDrawArrays(GL_TRIANGLE_STRIP, index*400, first * 3 - 2);
}
void Laser::Step(size_t index,float delta=1.0f/60.0f) {
	size_t off = index * 100;
	//MOVEMENT HERE
	angle += agSpeed;
	pos += speed * delta * vec2(sin(angle), cos(angle));

	auto ptr=laserMap.insert({ PosToMapIndex(pos), off+I(len) });
	mapPtrs[I(len)] = ptr;
	points[I(len)] = pos;

	//VERTEX CALCULATION
	float data[6];
	if (len > 0) {
		if (len == 1) {
			side = T(P(0), P(1)) * width;
			p = P(0) + side;
			glBufferSubData(GL_ARRAY_BUFFER, index * 800 * sizeof(GLfloat), 2 * sizeof(GLfloat), &p.x);
		}

		p = P(len - 1) - side;
		data[2] = p.x; data[3] = p.y;

		side = T(P(len - 1), P(len));
		side *= width;

		p = P(len) + side;
		data[0] = p.x; data[1] = p.y;

		width *= -1;
		side *= -1;

		p = P(len) + side;
		data[4] = p.x; data[5] = p.y;

		if (len + first == maxLen) {
			glBufferSubData(GL_ARRAY_BUFFER, index * 800 * sizeof(GLfloat), 2 * sizeof(GLfloat), &p.x);
		}
		glBufferSubData(GL_ARRAY_BUFFER, (index * 800+(1 + (len - 1 + first) % maxLen * 3) * 2) * sizeof(GLfloat), 6 * sizeof(GLfloat), data);

	}

	if (len < maxLen)len++; else {
		first++; first %= maxLen;
		laserMap.erase(mapPtrs[first]);
	}
	Render(index);
}
vector<Laser> vec_laser;

void LaserLoop() {//add delta
	static bool runOnce = false;
	if (!runOnce) {
		RenderInit();
		runOnce = true;
	}
	RenderBind();
	if (ImGui::IsKeyPressed(ImGuiKey_P)) {
		static size_t index = 0;
		Laser nl = Laser();
		nl.color = BLUE;
		nl.len = 0, nl.first = 0, nl.maxLen = 30;
		nl.pos = { -1.0f + .1f * index,0 };
		index++;
		nl.speed = 1;
		nl.agSpeed = .05f;
		nl.angle = .05f;
		nl.width = .03f;
		nl.points = new vec2[nl.maxLen];
		nl.mapPtrs = new unordered_multimap<int, unsigned int>::iterator[nl.maxLen];

		vec_laser.push_back(nl);

		//Resize
		GLint oldSize;
		glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &oldSize);
		void* oldData = malloc(oldSize);
		glGetBufferSubData(GL_ARRAY_BUFFER, 0, oldSize, oldData);

		glBufferData(GL_ARRAY_BUFFER, 800 * index * sizeof(GLfloat), NULL, GL_DYNAMIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, oldSize, oldData);
	}

	//COLLSION 

	BulletColorTag color = RED;
	for (int y = -100; y < 101; y += 100) {
		for (int x = -1; x < 2; x++) {
			auto range = laserMap.equal_range(PosToMapIndex(plPos) + x + y);
			for (auto it = range.first; it != range.second; ) {
				auto& laser = vec_laser[(float)it->second / 100.0f];
				if (glm::length(laser.points[it->second] - plPos) <= abs(laser.width) / 2) {//x* 1.1875 ???
					color = GREEN;
					break;
				}
				else
					++it;
			}
		}
	}

	//MOVEMENT
	for (size_t i = 0; i < vec_laser.size(); i++)
	{
		vec_laser[i].Step(i);
	}

	RenderUnBind();
}
