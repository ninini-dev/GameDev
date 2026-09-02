#include <own/bullet.h>

const GLfloat uAspects[] = {
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

const GLfloat uSizes[] = {
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
//SEGUIR EXPERIMENTANDO PARA GENERALIZAR TESTCOLLSION
/*
template <typename T>
void TestCollision(vec2 pos, unordered_multimap<int, unsigned int>& map, ComponentSystem<T> sys, void (*func)(const T&,
	const float& ,const unordered_multimap<int, unsigned int>&, const unordered_multimap<int, unsigned int>::iterator& )) {
	auto posIndex = PosToMapIndex(pos);
	for (int y = -100; y < 101; y += 100) {
		for (int x = -1; x < 2; x++) {
			auto range = map.equal_range(posIndex + x + y);
			for (auto it = range.first; it != range.second; ) {
				auto id = it->second;
				size_t page = id / MAX_PAGE_SIZE;
				size_t i = id % MAX_PAGE_SIZE;
				size_t index = sys.sparse[page][i]; 
				func(sys.dense[i], glm::length(sys.dense[index].pos - pos),map ,it);
			
			}
		}
	}
}

void BulletSystem::TestBTCol(const Bullet& bt, const float distance, 
	 unordered_multimap<int, unsigned int>& map, 
	 unordered_multimap<int, unsigned int>::iterator& it) {
	if (glm::length(bt.pos - pos) <= .05) {
		//bulletMap.erase(it);
		//cout << it->second<<endl;			auto auxID=denseToEntity[i];	
		dmg += bt.dmg;
		Remove(it->second);
		it = map.erase(it);

	}
	else
		++it;
}*/

float BulletSystem::TestBulletCollision(vec2 pos, unordered_multimap<int, unsigned int>& map){
	float dmg=0;
	auto posIndex = PosToMapIndex(pos);
	for (int y = -100; y < 101; y += 100) {
		for (int x = -1; x < 2; x++) {
			auto range = map.equal_range(posIndex + x + y);
			for (auto it = range.first; it != range.second; ) {
				auto id = it->second;
				size_t page = id / MAX_PAGE_SIZE;
				size_t i = id % MAX_PAGE_SIZE;
				size_t index = sparse[page][i];
				if (glm::length(dense[index].pos - pos) <= .05) {
					//bulletMap.erase(it);
					//cout << it->second<<endl;			auto auxID=denseToEntity[i];	
					dmg += dense[index].dmg;
					Remove(id);  
					it = map.erase(it);
				
				}else
				++it;
			}
		}
	}
	return dmg;
}
unordered_multimap<int, unsigned int> bulletMapColWEnm = {};
unordered_multimap<int, unsigned int> bulletMapColWPl = {};
void BulletSystem::Update(float delta) {
	ChangeTexture(2, uAspects, uSizes);
	size_t i = 0;
	while (i < dense.size())
	{
		float& ag = dense[i].angle;
		ag += dense[i].agSpeed;
		dense[i].pos += dense[i].speed* delta * vec2(sin(ag), cos(ag));
		if (abs(dense[i].pos.x) >= 1 || abs(dense[i].pos.y) >= 1) {
			BulletSystem::Delete(i);
			continue;
		}
		(dense[i].collidesWithEnemy?bulletMapColWEnm:bulletMapColWPl).insert({ PosToMapIndex(dense[i].pos),denseToEntity[i] });

		GLfloat data[] = {
			dense[i].pos.x,dense[i].pos.y,
			dense[i].aspect,dense[i].color,ag
		};
		sub(i, data);

		i++;
	}
	TestBulletCollision(plPos,bulletMapColWPl);

	bulletMapColWPl.clear();
	if (dense.size() > 0) rend(dense.size());

}