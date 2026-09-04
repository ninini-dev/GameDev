#pragma once
#define ENM_COL_TILES 48
#define MAX_COL_PER_CELL 32
#include <vector>
namespace Enemy {
	extern int col_map[ENM_COL_TILES * ENM_COL_TILES][MAX_COL_PER_CELL];
	extern int col_map_size[ENM_COL_TILES * ENM_COL_TILES];
	extern float b_x[4];
	extern float b_y[4];
}
void enemyLoop();
