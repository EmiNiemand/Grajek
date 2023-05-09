#ifndef GLOOMENGINE_SHOP_H
#define GLOOMENGINE_SHOP_H

#include "GameObjectsAndPrefabs/Prefab.h"
#include "Interfaces/SaveableStaticObject.h"
#include <memory>

class Shop : public Prefab,public SaveableStaticObject {
public:
    Shop(const std::string &name, int id, const std::shared_ptr<GameObject>& parent, Tags tag);
    ~Shop() override;

    std::shared_ptr<GameObject> Create() override;

    shared_ptr <StaticObjectData> SaveStatic() override;

    void LoadStatic(shared_ptr <StaticObjectData> &data) override;
};


#endif //GLOOMENGINE_SHOP_H
