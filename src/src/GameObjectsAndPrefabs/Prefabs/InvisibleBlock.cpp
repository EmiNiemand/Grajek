//
// Created by Mateusz on 01.06.2023.
//

#include "GameObjectsAndPrefabs/Prefabs/InvisibleBlock.h"
#include "Components/PhysicsAndColliders/BoxCollider.h"

/*In(const std::string &name, int id, const std::shared_ptr<GameObject> &parent, Tags tag) :
        Prefab(name, id, parent, tag) {
    prefabName = "Die";
}

Die::~Die() = default;

std::shared_ptr<GameObject> Die::Create() {
    auto die = shared_from_this();
    auto dieRenderer = die->AddComponent<Renderer>();
    dieRenderer->LoadModel("domek/domek.obj");
    auto dieCollider = die->AddComponent<BoxCollider>();
    dieCollider->SetOffset({0, 1, 0});

    return die;
}*/
InvisibleBlock::InvisibleBlock(const std::string &name, int id, const std::shared_ptr<GameObject> &parent, Tags tag) :
        Prefab(name, id, parent, tag) {
    prefabName = "InvisibleBlock";
}

InvisibleBlock::~InvisibleBlock() = default;

std::shared_ptr<GameObject> InvisibleBlock::Create() {
    auto block = shared_from_this();
    auto dieCollider = block->AddComponent<BoxCollider>();
    dieCollider->SetOffset({0, 1, 0});

    return block;
}

std::shared_ptr<StaticObjectData> InvisibleBlock::SaveStatic() {
    std::shared_ptr<StaticObjectData> newData = std::make_shared<StaticObjectData>();
    std::shared_ptr<BoxCollider> colider = GetComponent<BoxCollider>();

    newData->name = this->GetPrefabName();
    newData->uniqueName = this->GetName();
    newData->position = this->transform->GetLocalPosition();
    newData->rotation = this->transform->GetLocalRotation();
    newData->scale = this->transform->GetLocalScale();
    newData->coliderSize = colider->GetSize();
    newData->coliderOffset = colider->GetOffset();

    return newData;
}
