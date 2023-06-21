//
// Created by szymo on 17/06/2023.
//

#ifndef GLOOMENGINE_INDICATOR_H
#define GLOOMENGINE_INDICATOR_H

#include "GameObjectsAndPrefabs/Prefab.h"

class Indicator : public Prefab{
public:
    Indicator(const std::string &name, int id, const std::shared_ptr<GameObject> &parent, Tags tag);
    ~Indicator() override;

    std::shared_ptr<GameObject> Create() override;
};


#endif //GLOOMENGINE_INDICATOR_H
