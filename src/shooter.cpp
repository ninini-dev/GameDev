#include <own/shooter.h>
vector<ShooterParams> shooterList = {};
ShooterInstance::ShooterInstance(size_t i) {
	index = i;
	t = 0;
	angle = 0;
}
void ShooterInstance::Step(float delta, vec2 pos) {
	t -= delta;
	cout << "Step" << t<<endl;
	if (t <= 0) {
		ShooterParams p = shooterList[index];
		cout << p.bullet.pos.x<<"/"<< p.bullet.pos.y << endl;
		p.bullet.pos += pos;
		for (size_t i = 0; i < p.aA; i++)
		{
			p.bullet.angle = angle + p.sBtwA * i;
			BulletSystem::Add(CreateEntity(), p.bullet);
		}

		t += p.dBtwB;
		angle += p.rate;
	}
}