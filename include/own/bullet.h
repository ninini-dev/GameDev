#pragma once
#include <glm/vec2.hpp>

#include <own/entity.h>
#include <own/render.h>
#include <iostream>
#include <own/collision.h>
#include <own/pl.h>
#include <glm/geometric.hpp>
using namespace glm;

enum BulletAspectTag {
	LEAF,EMPTY_CIRCLE,CIRCLE,CAPSULE,KUNAI,OFUDA,BULLET,B_CAPSULE,START,GLOW,RING,B_TINY,TINY,FLASH,BULLET_ASPECT_COUNT
};
enum BulletColorTag {
	BLACK,D_RED,RED,D_MAGENTA,MAGENTA,D_BLUE,BLUE,D_CYAN,CYAN,D_GREEN,GREEN,LIME,D_YELLOW,YELLOW,GOLD,WHITE,BULLET_COLOR_COUNT
};
//		0.0625

namespace Nico {
	struct Bullet {
	public:
		BulletAspectTag aspect;
		BulletColorTag color;
		bool collidesWithEnemy;
		vec2 pos;
		float speed;
		float angle;
		float dmg;
		float agSpeed;
		Bullet() {
			this->aspect = CAPSULE;
			this->color = RED;
			this->collidesWithEnemy = false;
			this->pos = {0,0};
			this->speed = 1;
			this->angle = 0;
			this->dmg = 0;
			agSpeed = 0;
		}
		Bullet(BulletAspectTag aspect, BulletColorTag color, bool collidesWithEnemy, vec2 pos, float speed, float angle) {
			this->aspect = aspect;
			this->color = color;
			this->collidesWithEnemy = collidesWithEnemy;
			this->pos = pos;
			this->speed = speed;
			this->angle = angle;
			this->dmg = 10;
			agSpeed = 0;
		}
	};
}
using namespace std;
using namespace Nico;
class BulletSystem : ComponentSystem<Nico::Bullet> {
public:



	static void Create() {
		__super::Create();
	}
	static void Add(Entity e, Nico::Bullet obj) {
		__super::Add(e, obj);
	}
	static void Remove(Entity e) {
		__super::Remove(e);
	}
	static void Delete(size_t id) {
		__super::Delete(id);
	}
	static void Clear() {
		__super::Clear();
	}
	static float TestBulletCollision(vec2 pos, unordered_multimap<int, unsigned int>& map);
	
	static void Update(float delta);
};
vector<Nico::Bullet> BulletSystem::dense = {};
vector<Entity> BulletSystem::denseToEntity = {};
vector<vector<Entity>> BulletSystem::sparse = {};

