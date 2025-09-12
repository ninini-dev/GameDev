#pragma once
#include <own/sprite.h>
#include <own/bullet.h>
#include <iostream>
#include <span>
using namespace std;
enum EnemyAspectTag {
	TINY_BLUE, TINY_RED, TINY_GREEN, TINY_YELLOW,
	MAIND_BLUE, MAID_RED,
	DRESS_BLUE, DRESS_RED,
	SUN_RED,
	ENEMY_ASPECT_COUNT
};
/*
const Sprite enemyAspectDictionary[] = {
	 Sprite(0,192,32,32,12,512,512),Sprite(0,160,32,32,12,512,512),
	 Sprite(0,128,32,32,12,512,512),Sprite(0,96,32,32,12,512,512),

	 Sprite(0,512,48,32,4,512,512),Sprite(0,416,48,32,4,512,512),

	 Sprite(320,512,48,48,4,512,512),Sprite(320,368,48,48,4,512,512),

	 Sprite(0,64,64,64,4,512,512)
};
*/
struct ShooterParams {
public:
	Bullet bullet;
	unsigned int aB, aA;
	float dBtwB, dAtB,sBtwA,rate;
};

ShooterParams shooterList[] = {
	{Bullet(LEAF,RED,vec2(0,0),.1,0),1,1,.1,0,0,.1}
};
struct ShooterInstance {
public:
	unsigned int index;
	float t,angle;
	void Step(float delta,vec2 pos) {
		t -= delta;
		if (t <= 0) {
			ShooterParams p = shooterList[index];
			p.bullet.angle = angle;
			angle += p.rate;
			p.bullet.pos += pos;
			t += p.dBtwB;
			BulletSystem::Add(CreateEntity(), p.bullet);
		}
	}
};
#include <vector>
struct Enemy {
	vec2 pos;
	EnemyAspectTag aspect;
	int frame;
	vector<ShooterInstance> shooters;
	
	Enemy(vec2 pos, EnemyAspectTag aspect,vector<unsigned int> sh) {
		this->pos = pos;
		this->aspect = aspect;
		this->frame = 0;
		this->shooters = vector <ShooterInstance> {};
		
		for (size_t i = 0; i < sh.size(); i++)
		{
			shooters.push_back({sh[i],shooterList[sh[i]].dAtB,0});
		}

	}
};

const GLfloat uAspects[] = {
	0,	0.375,	0.0625,	0.0625,
	0,	0.3125,	0.0625,	0.0625,
	0,	.25,    0.0625,	0.0625,
	0,	0.1875,	0.0625,	0.0625,
	0,	1,	0.09375, 0.0625,
	0,	0.8125,	0.09375, 0.0625,
	0.625,	1, 0.09375, 0.09375,
	0.625,	0.71875, 0.09375, 0.09375,
	0,	0.125,	0.125,	0.125
};


const GLfloat uSizes[] = {
.08,.08,
.08,.08,
.08,.08,
.08,.08,
.12,.08,
.12,.08,
.12,.12,
.12,.12,
.16,.16
};


#include <own/entity.h>
#include <iostream>
class EnemySystem : ComponentSystem<Enemy> {
public:

	

	static void Create() {
		__super::Create();
	}
	static void Add(Entity e, Enemy enm) {
		__super::Add(e,enm);
	}
	static void Update(float delta) {
		ChangeTexture(1, uAspects, uSizes);
		for (size_t i = 0; i < dense.size(); i++)
		{
			dense[i].frame++;
			dense[i].frame %= 16;
			GLfloat data[] = {
				dense[i].pos.x,dense[i].pos.y,
				dense[i].aspect,dense[i].frame/4,0
			};

			const size_t length = dense[i].shooters.size();

			for (size_t j = 0; j < length; j++)
			{
				dense[i].shooters[j].Step(delta, dense[i].pos);
			}
			sub(i, data);
			
			
			
		}
		if(dense.size()>0) rend(dense.size());
	}
};
vector<vector<size_t>> EnemySystem::sparse = {};
vector<Enemy> EnemySystem::dense = {};
vector<Entity> EnemySystem::denseToEntity = {};
