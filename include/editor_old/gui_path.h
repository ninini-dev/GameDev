#pragma once
#include <vector>
#include <own/bezier.h>
using namespace std;
extern vector<vector<MovePath>> allPaths;
extern vector<MovePath>* curPath;
extern size_t curSeg;
extern MovePath* curCurve;
void SetCurCurve(size_t i);