//
// Created by masterktos on 01.04.23.
//

#ifndef GLOOMENGINE_PREFAB_H
#define GLOOMENGINE_PREFAB_H

#include <memory>
#include <string>

class GameObject;

class Prefab {
public:
    static std::shared_ptr<GameObject> GetPlayer(std::string name="Player");
    static std::shared_ptr<GameObject> GetCube(std::string name="Cube");
    static std::shared_ptr<GameObject> GetDancingDude(std::string name="dood");
};


#endif //GLOOMENGINE_PREFAB_H