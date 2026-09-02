#include <own/enemy.h>
const GLfloat uAspects[] = {
	0,	0.375,	0.0625,	0.0625,
	0,	0.3125,	0.0625,	0.0625,
	0,	.25,    0.0625,	0.0625,
	0,	0.1875,	0.0625,	0.0625,
	0,	1,	0.09375, 0.0625,
	0,	0.8125,	0.09375, 0.0625,
	0.625,	1, 0.09375, 0.09375,
	0.625,	0.71875, 0.09375, 0.09375,
	0,	0.125,	0.125,	0.125
};


const GLfloat uSizes[] = {
.08,.08,
.08,.08,
.08,.08,
.08,.08,
.12,.08,
.12,.08,
.12,.12,
.12,.12,
.16,.16
}; 
const float PI2 = acos(-1)*2; 

Enemy::Enemy() {
	this->drops = 0;
	this->shooters = {};
	this->tPath = 0;
}
Enemy::Enemy(vec2 pos, EnemyAspectTag aspect,vector<MovePath> paths,size_t drops, float hp){
	this->pos = pos;
	this->aspect = aspect;
	this->frame = 0;
	this->shooters = vector <ShooterInstance> {};
	this->pathQueue = vector<MovePath>(paths.rbegin(), paths.rend());
	this->drops = drops;
	//this->curPath.bezier.Displace(pos);
	this->tPath = 0;
	this->hp = hp;
}

#include <own/random.h>
void EnemySystem::Update(float delta)
{

	ChangeTexture(1, uAspects, uSizes);
	size_t i = 0;
	while (i < dense.size())
	{
		auto& enm = dense[i];
		//cout << enm.pos.x <<"/" << enm.pos.y << endl;
		MovementLoop(enm, delta);

		enm.frame++;
		enm.frame %= 16;
		GLfloat data[] = {
			enm.pos.x,enm.pos.y,
			enm.aspect,enm.frame / 4,0
		};

		//si una bala golpea a varios enemigos?
		enm.hp -= BulletSystem::TestBulletCollision(enm.pos, bulletMapColWEnm);
		

		if(enm.hp<=0){
			if (enm.drops < dropList.size()) {
				vector<Drop>& dv = dropList[enm.drops];
				for (size_t i = 0; i < dv.size(); i++)
				{
					Drop& d = dv[i];
					for (size_t i = 0; i < d.amount; i++)
					{
						auto a = nrand(0, PI2);
						auto l = nrand(d.rMin, d.rMax);
						vec2 pos = { cos(a) * l,sin(a) * l };
						pos += enm.pos;
						Item item = { d.item,pos,12.5,.75 };
						ItemSystem::Add(CreateEntity(), item);
					}
				}
			}
			else cout << "dropList index overflow!" << endl;
			Remove(denseToEntity[i]);
			i++;
			continue;
		}

		const size_t length = enm.shooters.size();
		for (size_t j = 0; j < length; j++)
		{
			enm.shooters[j].Step(delta, enm.pos);
		}

		sub(i, data);
		i++;


	}
	bulletMapColWEnm.clear();
	if (dense.size() > 0) rend(dense.size());
}
