//
// Created by Adrian on 07.05.2023.
//

#ifndef GLOOMENGINE_RHYTHMICCLAP_H
#define GLOOMENGINE_RHYTHMICCLAP_H

#include "GameObjectsAndPrefabs/Prefab.h"

class RhythmicClap : public Prefab {
public:
    RhythmicClap(const std::string &name, int id, const std::shared_ptr<GameObject>& parent, Tags tag);
    ~RhythmicClap() override;

    std::shared_ptr<GameObject> Create() override;
};

#endif //GLOOMENGINE_RHYTHMICCLAP_H
