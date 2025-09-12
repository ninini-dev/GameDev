#pragma once
#include <queue>

using namespace std;

using Entity = uint32_t;
Entity entitiesCount;
queue<Entity> avalibleEntities;
void ExpandEntities() {
	for (Entity i = entitiesCount; i < entitiesCount + 100; i++)
	{
		avalibleEntities.push(i);
	}

	entitiesCount += 100;
}
Entity CreateEntity() {
	if (avalibleEntities.size() == 0)ExpandEntities();
	Entity front = avalibleEntities.front();
	avalibleEntities.pop();
	return front;
}
template<typename T>
class ComponentSystem {
public:
	static const Entity MAX_ENTITIES = 5000;
	static const Entity MAX_PAGE_SIZE = 50;

	static vector<vector<size_t>> sparse;
	static vector<T> dense;
	static vector<Entity> denseToEntity;

	static void Add(Entity id, T item);
	static void Update(float delta);
	static void Create();
	static void Remove(Entity e);

	static void DeleteDense(size_t index);

}; 
template<typename T>
void ComponentSystem<T>::Create() {
	for (size_t i = 0; i < MAX_ENTITIES / MAX_PAGE_SIZE; i++)
		sparse.push_back({ 0 });
}
template<typename T>
void ComponentSystem<T>::DeleteDense(size_t index) {
	swap(dense[index], dense.back());
	swap(denseToEntity[index], denseToEntity.back());
	dense.pop_back();
	denseToEntity.pop_back();
}

template<typename T>
void ComponentSystem<T>::Add(Entity id, T item) {
	
	size_t page = id / MAX_PAGE_SIZE;
	size_t i = id % MAX_PAGE_SIZE;
	while (i >= sparse[page].size())sparse[page].push_back(-1);
	sparse[page][i] = dense.size();
	dense.push_back(item);
	denseToEntity.push_back(id);
	//size++;
	
}
template<typename T>
void ComponentSystem<T>::Remove(Entity id) {
	size_t page = id / MAX_PAGE_SIZE;
	size_t i = id % MAX_PAGE_SIZE;
	size_t index = sparse[page][i];

	Entity back = denseToEntity.back();
	swap(dense[index], dense.back());
	swap(denseToEntity[index], denseToEntity.back());

	sparse[page][i] = -1;
	page = back / MAX_PAGE_SIZE;
	i = back % MAX_PAGE_SIZE;
	sparse[page][i] = index;

	dense.pop_back();
	denseToEntity.pop_back();
	size--;
}