//
// Created by Adrian on 07.05.2023.
//

#ifndef GLOOMENGINE_JAZZCLAP_H
#define GLOOMENGINE_JAZZCLAP_H

#include "GameObjectsAndPrefabs/Prefab.h"

class JazzClap : public Prefab {
public:
    JazzClap(const std::string &name, int id, const std::shared_ptr<GameObject>& parent, Tags tag);
    ~JazzClap() override;

    std::shared_ptr<GameObject> Create() override;
};

#endif //GLOOMENGINE_JAZZCLAP_H
