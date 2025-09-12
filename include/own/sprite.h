#pragma once
#include <vector>
#include <glm/vec2.hpp>
#include <glad/glad.h>

using namespace std;
using namespace glm;

struct Sprite {
	float x, ox, y, oy, w, h;
	int columns, frame, step;
	Sprite() {
		x = 0; y = 0;
		ox = 0; oy = 0;
		w = 0; h = 0;
		columns = 0; frame = 0; step = 0;
	}
	Sprite(float x, float y, float w, float h, int c, float texW = 1, float texH = 1) {
		this->x = x / texW;
		this->y = y / texH;
		this->ox = w / 400;
		this->oy = h / 400;
		this->w = w / texW;
		this->h = h / texH;
		this->columns = c;
		this->frame = 0;
		this->step = 4;
	}
	GLfloat* GetVector(vec2 pos) {
		float ow = w * (frame / step % columns);
		float oh = h * (frame / step / columns);
		/*
		GLfloat vVec[8] = {
				x + ow,			y - h - oh,
			x + ow + w,		y - h - oh,
			x + ow,			y - oh,
					x + ow + w,		y - oh,
		};*/
		
		GLfloat vVec[] = {
			pos.x - ox * .5f,			pos.y - oy * .5f,			x + ow,			y - h - oh,
			pos.x + ox * .5f,		pos.y - oy * .5f,			x + ow + w,		y - h - oh,
			pos.x - ox * .5f,			pos.y + oy * .5f,		x + ow,			y - oh,
			pos.x + ox * .5f,		pos.y + oy * .5f,		x + ow + w,		y - oh,
		};
		return vVec;
	}
};
