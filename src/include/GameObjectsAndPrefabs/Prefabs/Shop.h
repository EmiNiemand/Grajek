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

    std::shared_ptr<StaticObjectData> SaveStatic() override;
};


#endif //GLOOMENGINE_SHOP_H
