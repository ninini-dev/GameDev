#include <editor/editor.h>
#include <own/enemy.h>
#include <fstream>
#include <iostream>
#include <editor/gui_drop.h>


void BuildStageFile(Enemy enm) {

    std::ofstream outFile("data/enm.enm", std::ios::binary);

    if (outFile.is_open()) {
        outFile.write(reinterpret_cast<char*>(&enm.aspect), sizeof(EnemyAspectTag));
        outFile.write(reinterpret_cast<char*>(&enm.hp), sizeof(float));
       
        auto dropIndex = std::distance(dropsList.data(), enm.drops);
        outFile.write(reinterpret_cast<char*>(&dropIndex), sizeof(size_t));
        
        
        auto pathQueueSize = enm.pathQueue.size();
        outFile.write(reinterpret_cast<char*>(&pathQueueSize),sizeof(size_t));
        for (size_t i = 0; i < pathQueueSize; i++)
            outFile.write(reinterpret_cast<char*>(&enm.pathQueue[i]), sizeof(MovePath));
        
        
        size_t shSize = enm.shooters.size();
        outFile.write(reinterpret_cast<char*>(&shSize), sizeof(size_t));
        for (size_t i = 0; i < shSize; i++)
            outFile.write(reinterpret_cast<char*>(&enm.shooters[i].index), sizeof(unsigned int));
        
        outFile.close();
        std::cout << "Data written" << std::endl;
    }
    else {
        std::cerr << "Error opening file!" << std::endl;
    }
}
void ReadStageFile() {
    Enemy readedEnemy;

    readedEnemy.pos = { 0,0 };

    std::ifstream inputFile("data/enm.enm", std::ios::binary | std::ios::in);

    inputFile.read(reinterpret_cast<char*>(&readedEnemy.aspect), sizeof(EnemyAspectTag));
    inputFile.read(reinterpret_cast<char*>(&readedEnemy.hp), sizeof(float)); 

    size_t dropIndex;
    inputFile.read(reinterpret_cast<char*>(&dropIndex), sizeof(size_t));
    readedEnemy.drops = &dropsList[dropIndex];

    size_t size;
    inputFile.read(reinterpret_cast<char*>(&size), sizeof(size_t));

    readedEnemy.pathQueue = vector<MovePath>(size);
    inputFile.read(reinterpret_cast<char*>(readedEnemy.pathQueue.data()), size);

    inputFile.read(reinterpret_cast<char*>(&size), sizeof(size_t));

    auto ids= vector<unsigned int>(size);
    inputFile.read(reinterpret_cast<char*>(readedEnemy.shooters.data()), size);

    inputFile.close();
    EnemySystem::Add(CreateEntity(), readedEnemy);
}