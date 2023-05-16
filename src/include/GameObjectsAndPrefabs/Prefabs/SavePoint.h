#ifndef IMAGE_CPP_SAVEPOINT_H
#define IMAGE_CPP_SAVEPOINT_H

#include "GameObjectsAndPrefabs/Prefab.h"

class SavePoint : public Prefab {
public:
    SavePoint(const std::string &name, int id, const std::shared_ptr<GameObject>& parent, Tags tag);
    ~SavePoint() override;

    std::shared_ptr<GameObject> Create() override;
};

#endif //IMAGE_CPP_SAVEPOINT_H