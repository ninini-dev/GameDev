#pragma once
#define MAX_PL 300
extern float pl_x;
extern float pl_y;
extern bool pl_keys[1024];
void playerLoop();
namespace Player {
	extern float x[MAX_PL];
	extern float y[MAX_PL];
	extern int count;
}
void delete_ptama(int i);