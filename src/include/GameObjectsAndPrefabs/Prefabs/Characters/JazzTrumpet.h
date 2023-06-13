//
// Created by Adrian on 07.05.2023.
//

#ifndef GLOOMENGINE_JAZZTRUMPET_H
#define GLOOMENGINE_JAZZTRUMPET_H

#include "GameObjectsAndPrefabs/Prefab.h"

class JazzTrumpet : public Prefab {
public:
    JazzTrumpet(const std::string &name, int id, const std::shared_ptr<GameObject>& parent, Tags tag);
    ~JazzTrumpet() override;

    std::shared_ptr<GameObject> Create() override;
};

#endif //GLOOMENGINE_JAZZTRUMPET_H
