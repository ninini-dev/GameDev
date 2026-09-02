#include <own/stage.h>
void CreateEnemy(Enemy enm) {
	EnemySystem::Add(CreateEntity(), enm);
}
StageEvent::StageEvent() {
	this->positions = { {0,0} };
	this->time = 0;
	this->it = 0;
	this->amount = 1;
	//this->enm = Enemy();
}
int StageEvent::Step(float delta) {
	time += delta;
	if (time >= delayInBetween) {
		time -= delayInBetween;
		auto pos = positions[it % positions.size()];
		//enm.pos = pos;
		Enemy newEnm = enm;
		
		for (size_t i = 0; i < newEnm.pathQueue.size(); i++)
		{
			newEnm.pathQueue[i].bezier.Displace(pos);
			//cout << newEnm.pos.x<<" | "<<newEnm.pos.y<<endl;
		}
		CreateEnemy(newEnm);
		cout << "new" << endl;
		amount--;
		if (amount<=0)return 1;
		it++;
	}
	return 0;
}
float timer=0;
queue<StageEvent> waitingEvents;
vector<StageEvent> activeEvents;



void ClearEvents() {

	queue<StageEvent> empty;
	swap(waitingEvents, empty);
	activeEvents.clear();
}
void ClearStage() {
	EnemySystem::Clear();
	BulletSystem::Clear();
	ItemSystem::Clear();

	ClearEvents();
}
void AddEvent(StageEvent event) {

	cout << "event" << endl;
	waitingEvents.push(event);
}
void StageEventLoop(float delta) {

	timer += delta;
	if(!waitingEvents.empty())
	if (waitingEvents.front().time <= timer) {

		cout << "load" << endl;
		waitingEvents.front().time = waitingEvents.front().delayInBetween;
		activeEvents.push_back(waitingEvents.front());
		waitingEvents.pop();
	}
	size_t i = 0;
	while (i < activeEvents.size()) {
		if (activeEvents[i].Step(delta))activeEvents.erase(activeEvents.begin() + i);
		i++;
	}
}