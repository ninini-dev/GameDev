#pragma once
#include <glm/vec2.hpp>
#include <own/render.h>
#include <own/collision.h>
#include <unordered_map>
#include <own/bullet.h>
using namespace glm;
vec2 plPos = { .02,.02 };
void PlLoop() {
	static const GLfloat plAspect[] = {
	0,1,.125,.2
	}, 
	plSize[] = {
	.08,.128
	};
	ChangeTexture(3, plAspect, plSize);
	GLfloat data[] = {
		plPos.x,plPos.y,
		0,0,0
	};
	sub(0,data);
	rend(1);
	auto range = bulletMap.equal_range(PosToMapIndex(plPos));
	for (auto it = range.first;it != range.second;++it) {
		//cout << it->second << endl;
		
	}
}