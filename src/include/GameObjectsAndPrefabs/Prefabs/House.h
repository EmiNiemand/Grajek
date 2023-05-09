#ifndef GLOOMENGINE_HOUSE_H
#define GLOOMENGINE_HOUSE_H

#include "GameObjectsAndPrefabs/Prefab.h"

class House : public Prefab {
public:
    House(const std::string &name, uint32_t id, const std::shared_ptr<GameObject>& parent, Tags tag);
    ~House() override;

    std::shared_ptr<GameObject> Create() override;
};


#endif //GLOOMENGINE_HOUSE_H
