#ifndef GLOOMENGINE_DIE_H
#define GLOOMENGINE_DIE_H

#include "GameObjectsAndPrefabs/Prefab.h"

class Die : public Prefab {
public:
    Die(const std::string &name, int id, const std::shared_ptr<GameObject>& parent, Tags tag);
    ~Die() override;

    std::shared_ptr<GameObject> Create() override;
};


#endif //GLOOMENGINE_DIE_H
