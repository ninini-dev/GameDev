#include <own/item.h>
#include <own/render.h>

vector<vector<Drop>> dropList = { };

const GLfloat uAspects[] = {
	0,	0, 0.125,	.5,
	0.125,	0, 0.125,	.5,
	0.25,	0, 0.125,	.5,
	0.375,	0, 0.125,	.5,
	0.5,	0, 0.125,	.5,
	0.625,	0, 0.125,	.5,
	.75,	0, 0.0625,	.25,
	0.8125,	0, 0.0625,	.25
};


const GLfloat uSizes[] = {
.08,.08,
.08,.08,
.08,.08,
.08,.08,
.08,.08,
.08,.08,
.04,.04,
.04,.04
};
#include <own/collision.h>
#include <glm/geometric.hpp>
#include <own/pl.h>
unordered_multimap<int, unsigned int> itemMapCol = {};
float ItemSystem::TestCollsion() {
	float dmg = 0;
	auto posIndex = PosToMapIndex(plPos);
	for (int y = -100; y < 101; y += 100) {
		for (int x = -1; x < 2; x++) {
			auto range = itemMapCol.equal_range(posIndex + x + y);
			for (auto it = range.first; it != range.second; ) {
				auto id = it->second;
				size_t page = id / MAX_PAGE_SIZE;
				size_t i = id % MAX_PAGE_SIZE;
				size_t index = sparse[page][i];
				if (glm::length(dense[index].pos - plPos) <= .1) {
					//bulletMap.erase(it);
					//cout << it->second<<endl;			auto auxID=denseToEntity[i];	
					Remove(id);
					it = itemMapCol.erase(it);

				}
				else
					++it;
			}
		}
	}
	return dmg;
}
#include <own/pl.h>
void ItemSystem::Update(float delta) {

	ChangeTexture(4, uAspects, uSizes);
	size_t i = 0;

	while (i < dense.size())
	{
		auto& item = dense[i];

		if (item.angle > 0) {
			item.angle -= delta*25;
			if (item.angle < 0)item.angle = 0;
			
		}
		if (item.speed > -.75) {
			item.speed -= delta;
		}
		item.pos.y += item.speed*delta;

		itemMapCol.insert({ PosToMapIndex(dense[i].pos),denseToEntity[i] });

		GLfloat data[] = {
			item.pos.x,item.pos.y,
			item.aspect,0,item.angle
		};
		sub(i, data);
		i++;
	}
	ItemSystem::TestCollsion();
	itemMapCol.clear();
	if (dense.size() > 0) rend(dense.size());

}