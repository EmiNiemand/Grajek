//
// Created by Adrian on 05.05.2023.
//

#ifndef GLOOMENGINE_CHARACTERS_H
#define GLOOMENGINE_CHARACTERS_H

#include "GameObjectsAndPrefabs/Prefab.h"

class Default : public Prefab {
public:
    Default(const std::string &name, int id, const std::shared_ptr<GameObject>& parent, Tags tag);
    ~Default() override;

    std::shared_ptr<GameObject> Create() override;
};

#endif //GLOOMENGINE_CHARACTERS_H
