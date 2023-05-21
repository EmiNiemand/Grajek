//
// Created by Mateusz on 19.05.2023.
//

#ifndef GLOOMENGINE_GAMEOBJECTDATA_H
#define GLOOMENGINE_GAMEOBJECTDATA_H

#include "GameObjectsAndPrefabs/GameObject.h"

class GameObjectData {
    std::string name;
    std::shared_ptr<GameObject> parent;
    Tags TAGS;
    std::vector<std::string> componentNamesList;
};


#endif //GLOOMENGINE_GAMEOBJECTDATA_H
