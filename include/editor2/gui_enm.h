#pragma once
#include <bitset>
#include <own/enemy.h>
void OpenEnmWindow();
void EnmWindow();
bool TestPath(int i);
bool TestShooter(size_t i);
void UpdateDrop(size_t drop);
void UpdatePath(int i);
void UpdateShooters(int i, bool push);
bitset<32>& GetShooters();