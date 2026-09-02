#pragma once
#include <own/bullet.h>
struct ShooterParams {
public:
	Nico::Bullet bullet;
	unsigned int aB, aA;
	float dBtwB, dAtB, sBtwA, rate;
	// a Mount	|	d	Delay	|	Btw		Between	|
};

extern vector<ShooterParams> shooterList;
struct ShooterInstance {
public:
	size_t index;
	float t, angle;
	ShooterInstance(size_t i);
	void Step(float delta, vec2 pos);

	bool operator==(const ShooterInstance& other) const {
		return index == other.index;
	}
};