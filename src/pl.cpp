#include <own/pl.h>
#include <own/bullet.h>
#include <own/render.h>
vec2 plPos = { -.5f,0 };
void PlLoop() {
	static const GLfloat plAspect[] = {
	0,1,.125,.2
	},
		plSize[] = {
		.08,.128
	};
	ChangeTexture(3, plAspect, plSize);
	GLfloat data[] = {
		plPos.x,plPos.y,
		0,0,0
	};
	sub(0, data);
	rend(1);

	static int shootDelay = 4;
	static Bullet bullet = Bullet(BulletAspectTag::CIRCLE, BLUE,true, vec2(0, 0), 3, 0);
	shootDelay--;
	if (shootDelay <= 0) {
		shootDelay = 4;
		bullet.pos = plPos;
		BulletSystem::Add(CreateEntity(), bullet);

	}

}