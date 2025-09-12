#pragma once
#include <unordered_map>
#include <own/entity.h>
#include <glm/vec2.hpp>
using namespace std;
using namespace glm;
unordered_multimap<int, unsigned int> bulletMap = {};

int PosToMapIndex(vec2 pos) {
	return (pos.x * 10) + (pos.y *10) * 100;
}