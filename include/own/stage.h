#pragma once
#include <own/enemy.h>
#include <queue>

struct StageEvent {
	float time;
	float delayInBetween;
	int amount;
	size_t it;
	Enemy enm;
	vector<vec2> positions;
	int Step(float delta);
	StageEvent();
};
void AddEvent(StageEvent event);
void StageEventLoop(float delta);
void ClearStage();