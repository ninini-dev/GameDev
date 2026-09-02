#pragma once
#include <unordered_map>
#include <own/entity.h>
#include <glm/vec2.hpp>
using namespace std;
using namespace glm;
extern unordered_multimap<int, unsigned int> bulletMapColWEnm, bulletMapColWPl;

int PosToMapIndex(vec2 pos);