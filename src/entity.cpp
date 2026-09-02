#include <own/entity.h>

Entity entitiesCount;
queue<Entity> avalibleEntities;

void ExpandEntities() {
	for (Entity i = entitiesCount; i < entitiesCount + 100; i++)
	{
		avalibleEntities.push(i);
	}
	//cout << "EXPAND ENTITIES";
	entitiesCount += 100;

}

Entity CreateEntity() {
	if (avalibleEntities.size() == 0)ExpandEntities();
	Entity front = avalibleEntities.front();
	avalibleEntities.pop();
	return front;
}