//
// Created by Mateusz on 01.06.2023.
//

#ifndef GLOOMENGINE_INVISIBLEBLOCK_H
#define GLOOMENGINE_INVISIBLEBLOCK_H

#include "GameObjectsAndPrefabs/Prefab.h"
#include "Interfaces/SaveableStaticObject.h"

class InvisibleBlock : public Prefab, public SaveableStaticObject {
public:
    InvisibleBlock(const std::string &name, int id, const std::shared_ptr<GameObject>& parent, Tags tag);
    ~InvisibleBlock() override;

    std::shared_ptr<GameObject> Create() override;

    std::shared_ptr<StaticObjectData> SaveStatic() override;
};


#endif //GLOOMENGINE_INVISIBLEBLOCK_H
