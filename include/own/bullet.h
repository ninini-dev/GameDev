#pragma once
#include <glm/vec2.hpp>
using namespace glm;

enum BulletAspectTag {
	LEAF,EMPTY_CIRCLE,CIRCLE,CAPSULE,KUNAI,OFUDA,BULLET,B_CAPSULE,START,GLOW,RING,B_TINY,TINY,FLASH,BULLET_ASPECT_COUNT
};
enum BulletColorTag {
	BLACK,D_RED,RED,D_MAGENTA,MAGENTA,D_BLUE,BLUE,D_CYAN,CYAN,D_GREEN,GREEN,LIME,D_YELLOW,YELLOW,GOLD,WHITE,BULLET_COLOR_COUNT
};
//		0.0625
const GLfloat uAspects2[] = {
	0,		.9375,		.0625,		.0625,
	0,		.875,		.0625,		.0625,
	0,		.8125,		.0625,		.0625,
	0,		.75,		.0625,		.0625,
	0,		.6875,		.0625,		.0625,
	0,		.625,		.0625,		.0625,
	0,		.5625,		.0625,		.0625,
	0,		.5,			.0625,		.0625,
	0,		.4375,		.0625,		.0625,
	0,		.375,		.0625,		.0625,
	0,		.3125,		.0625,		.0625,
	0,		.28125,		.0625,		.0625,
	0,		.25,		.0625,		.0625
};

const GLfloat uSizes2[] = {
	.04,	.04,
	.04,	.04,
	.04,	.04,
	.04,	.04,
	.04,	.04,
	.04,	.04,
	.04,	.04,
	.04,	.04,
	.04,	.04,
	.04,	.04,
	.04,	.04,
	.04,	.04,
	.04,	.04,
};

struct Bullet {
	public:	
		BulletAspectTag aspect;
		BulletColorTag color;
		vec2 pos;
		float speed;
		float angle;
		Bullet(BulletAspectTag aspect, BulletColorTag color, vec2 pos,float speed,float angle) {
			this->aspect = aspect;
			this->color = color;
			this->pos = pos;
			this->speed = speed;
			this->angle = angle;
	}
};

#include <own/entity.h>
#include <own/render.h>
#include <own/collision.h>
#include <iostream>
using namespace std;
class BulletSystem : ComponentSystem<Bullet> {
public:



	static void Create() {
		__super::Create();
	}
	static void Add(Entity e, Bullet obj) {
		__super::Add(e, obj);
	}
	static void DeleteDense(int index) {
		__super::DeleteDense(index);
	}
	static void Update(float delta) {
		bulletMap.clear();
		ChangeTexture(2, uAspects2, uSizes2);
		
		size_t i = 0;
		while ( i < dense.size())
		{
			float ag = dense[i].angle;
			dense[i].pos += delta*vec2(sin(ag),cos(ag));
			if (dense[i].pos.x >= 1 || dense[i].pos.y >= 1) {
				BulletSystem::DeleteDense(i);
				continue;
			}
			
			bulletMap.insert({ PosToMapIndex(dense[i].pos),denseToEntity[i]});
			
			GLfloat data[] = {
				dense[i].pos.x,dense[i].pos.y,
				dense[i].aspect,dense[i].color,ag
			};
			sub(i, data);

			i++;
		}
		
		if (dense.size() > 0) rend(dense.size());

	}
};
vector<vector<size_t>> BulletSystem::sparse = {};
vector<Bullet> BulletSystem::dense = {};
vector<Entity> BulletSystem::denseToEntity = {};

