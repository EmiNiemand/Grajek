//
// Created by szymo on 17/06/2023.
//

#ifndef GLOOMENGINE_CONEINDICATOR_H
#define GLOOMENGINE_CONEINDICATOR_H

#include "GameObjectsAndPrefabs/Prefab.h"

class ConeIndicator : public Prefab{
public:
    ConeIndicator(const std::string &name, int id, const std::shared_ptr<GameObject> &parent, Tags tag);
    ~ConeIndicator() override;

    std::shared_ptr<GameObject> Create() override;
};


#endif //GLOOMENGINE_CONEINDICATOR_H
