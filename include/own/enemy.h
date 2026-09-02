#pragma once
#include <own/sprite.h>
#include <own/bullet.h>
#include <iostream>
#include <own/bezier.h>
using namespace std;
typedef enum EnemyAspectTag {
	TINY_BLUE, TINY_RED, TINY_GREEN, TINY_YELLOW,
	MAIND_BLUE, MAID_RED,
	DRESS_BLUE, DRESS_RED,
	SUN_RED,
	ENEMY_ASPECT_COUNT
}; 
#include <vector>
#include <own/item.h>
#include <own/shooter.h>
struct Enemy {
	
	//aspect
	EnemyAspectTag aspect;
	int frame;
	//Shoot
	vector<ShooterInstance> shooters;
	//Movement 
	vec2 pos;
	vector<MovePath> pathQueue;
	float tPath;
	size_t drops;
	float hp;
	//Func
	Enemy();
	Enemy(vec2 pos, EnemyAspectTag aspect, vector<MovePath> paths,size_t drops, float hp);
};


#include <own/entity.h>
#include <iostream>
class EnemySystem : ComponentSystem<Enemy> {
public:
	static void Remove(Entity id) {
		__super::Remove(id);
	}
	static void Create() {
		__super::Create();
	}
	static void Add(Entity e, Enemy enm) {
		__super::Add(e,enm);
	}
	static void Clear() {
		__super::Clear();
	}
	static void MovementLoop(Enemy& enm, const float delta) {
		if (!enm.pathQueue.empty()) {
			auto& path = enm.pathQueue.back();
			auto& bez = path.bezier;
			enm.tPath += delta;
			cout << enm.tPath << endl;
			if (enm.tPath >= path.duration) {

				enm.tPath -= path.duration;
				enm.pathQueue.pop_back();
				if (enm.pathQueue.empty()) {
					enm.pos = path.bezier.p3;
					return;
				}	
				path = enm.pathQueue.back();
				bez = path.bezier;
			}
			enm.pos = bez.Calculate(enm.tPath / path.duration, path.ease);
		}
	}
	static void Update(float delta);
};
vector<vector<Entity>> EnemySystem::sparse = {};
vector<Enemy> EnemySystem::dense = {};
vector<Entity> EnemySystem::denseToEntity = {};
