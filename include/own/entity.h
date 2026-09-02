#pragma once
#include <queue>
#include <iostream>
using namespace std;

using Entity = uint32_t;
extern Entity entitiesCount;
extern queue<Entity> avalibleEntities;

const Entity MAX_ENTITIES = 100000;
const Entity MAX_PAGE_SIZE = 100;

void ExpandEntities();
Entity CreateEntity();

template<typename T>
class ComponentSystem {
public:
	static vector<T> dense;
	static vector<Entity> denseToEntity;
	static vector<vector<Entity>> sparse;

	static void Add(Entity id, T item);
	static void Update(float delta);
	static void Create();
	static void Remove(Entity id);
	static void Delete(size_t pos);
	static void Clear();

}; 

template<typename T>
void ComponentSystem<T>::Create() {
	for (size_t i = 0; i < MAX_ENTITIES / MAX_PAGE_SIZE; i++)
		sparse.push_back({ 0 });
}

template<typename T>
void ComponentSystem<T>::Delete(size_t pos) {
	auto id = denseToEntity[pos];
	Remove(id);
}

template<typename T>
void ComponentSystem<T>::Clear() {
	for (size_t i = 0; i < denseToEntity.size(); i++)
	{
		avalibleEntities.push(denseToEntity[i]);
	}
	dense.clear();
	denseToEntity.clear();
	sparse.clear();
	Create();
}


template<typename T>
void ComponentSystem<T>::Remove(Entity id) {
	avalibleEntities.push(id); //SI LAS ENTITDADES TIENEN QUE REMOVER VARIOS COMPONENTES PUEDE SER QUE ESTA LINEA SE EJECUTE
	size_t page = id / MAX_PAGE_SIZE;
	size_t i = id % MAX_PAGE_SIZE;
	size_t index = sparse[page][i];

	Entity back = denseToEntity.back();
	if (dense.size() <= index)cout <<"ERROR DENSE INDEX:"<<index << endl;
	swap(dense[index], dense.back());
	swap(denseToEntity[index], denseToEntity.back());

	sparse[page][i] = -1;
	page = back / MAX_PAGE_SIZE;
	i = back % MAX_PAGE_SIZE;
	sparse[page][i] = index;

	dense.pop_back();
	denseToEntity.pop_back();
}

#include <iostream>
template<typename T>
void ComponentSystem<T>::Add(Entity id, T item) {
	size_t page = id / MAX_PAGE_SIZE;
	size_t i = id % MAX_PAGE_SIZE;
	if (page >= sparse.size())cout << "SPARSE PAGE ERROR";
	while (i >= sparse[page].size()) { sparse[page].push_back(-1); }
	sparse[page][i] = dense.size();

	dense.push_back(item);
	denseToEntity.push_back(id);
	
}
