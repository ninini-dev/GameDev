#pragma once
#include <own/entity.h>
#include <iostream>
#include <glm/vec2.hpp>
using namespace std;
using namespace glm;

typedef enum ItemAspectTag {
	POWER_UP, HP_FRAG, HP_UP, BOMB_FRAG, BOMB_UP, FULL,
	POWER, FAITH, ITEM_ASPECT_COUNT
};




typedef struct Item {
	ItemAspectTag aspect;
	vec2 pos;
	float angle;
	float speed;
};
typedef struct Drop {
	ItemAspectTag item;
	int amount;
	float rMin, rMax;
};
#include <unordered_map>
class ItemSystem : ComponentSystem<Item> {
public:


	static void Remove(Entity id) {
		__super::Remove(id);
	}
	static void Create() {
		__super::Create();
	}
	static void Add(Entity e, Item enm) {
		__super::Add(e, enm);
	}
	static void Clear() {
		__super::Clear();
	}
	static float TestCollsion();
	static void Update(float delta); 
};
vector<vector<Entity>> ItemSystem::sparse = {};
vector<Item> ItemSystem::dense = {};
vector<Entity> ItemSystem::denseToEntity = {};
extern vector<vector<Drop>> dropList;