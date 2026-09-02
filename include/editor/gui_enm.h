#pragma once
#include <bitset>
#include <own/enemy.h>
struct EnemyRegistry {
	size_t drop;
	size_t path;
	bitset<32> sh;
	float hp;
	EnemyAspectTag aspect;
};
void UpdateDrop(size_t drop);
void EnmWindow();
void OpenEnmWindow(); 
void UpdateShooters(size_t pos);
void ShiftShooterSelection(size_t pos);
void UpdatePath(size_t path);	
EnemyRegistry GetEnm(size_t id);