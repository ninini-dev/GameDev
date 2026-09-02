#include <own/collision.h>


int PosToMapIndex(vec2 pos) {
	return (int)(pos.x * 10) + (int)(pos.y * 10) * 100;
}
