#ifndef GLOOMENGINE_PLAYER_H
#define GLOOMENGINE_PLAYER_H

#include "GameObjectsAndPrefabs/Prefab.h"

class Player : public Prefab {
public:
    Player(const std::string &name, uint32_t id, const std::shared_ptr<GameObject>& parent, Tags tag);
    ~Player() override;

    std::shared_ptr<GameObject> Create() override;
};


#endif //GLOOMENGINE_PLAYER_H
