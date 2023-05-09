//
// Created by Adrian on 07.05.2023.
//

#ifndef GLOOMENGINE_ROCKDRUMS_H
#define GLOOMENGINE_ROCKDRUMS_H

#include "GameObjectsAndPrefabs/Prefab.h"

class RockDrums : public Prefab {
public:
    RockDrums(const std::string &name, int id, const std::shared_ptr<GameObject>& parent, Tags tag);
    ~RockDrums() override;

    std::shared_ptr<GameObject> Create() override;
};

#endif //GLOOMENGINE_ROCKDRUMS_H
