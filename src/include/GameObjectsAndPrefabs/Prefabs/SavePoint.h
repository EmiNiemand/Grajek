#ifndef IMAGE_CPP_SAVEPOINT_H
#define IMAGE_CPP_SAVEPOINT_H

#include "GameObjectsAndPrefabs/Prefab.h"
#include "Interfaces/SaveableStaticObject.h"

class SavePoint : public Prefab,public SaveableStaticObject {
public:
    SavePoint(const std::string &name, int id, const std::shared_ptr<GameObject>& parent, Tags tag);
    ~SavePoint() override;

    std::shared_ptr<GameObject> Create() override;

    std::shared_ptr<StaticObjectData> SaveStatic() override;
};

#endif //IMAGE_CPP_SAVEPOINT_H