#include "GameObjectsAndPrefabs/Prefabs/SavePoint.h"
#include "Components/Renderers/Renderer.h"
#include "Components/PhysicsAndColliders/BoxCollider.h"
#include "Components/Scripts/Menus/SavePointTrigger.h"
#include "Components/Animations/GameObjectAnimator.h"

SavePoint::SavePoint(const std::string &name, int id, const std::shared_ptr<GameObject> &parent, Tags tag) : Prefab(
        name, id, parent, tag) {
    prefabName = "SavePoint";
}

SavePoint::~SavePoint() = default;

std::shared_ptr<GameObject> SavePoint::Create() {
    auto savePoint = shared_from_this();
    savePoint->AddComponent<Renderer>()->LoadModel("Buildings/Environment/PostersColumn.obj");
    savePoint->transform->SetLocalScale(glm::vec3(0.25));
    savePoint->AddComponent<BoxCollider>()->SetOffset({0, 0, 0});
    std::shared_ptr<GameObject> s = GameObject::Instantiate("S", savePoint);
    s->AddComponent<Renderer>()->LoadModel("Buildings/Environment/floppyDisk.obj");
    s->AddComponent<GameObjectAnimator>()->Setup(s->transform, {
            {AnimatedProperty::Rotation, glm::vec3(0.0f, -360.0f, 0.0f), 5.0f},
    }, true);

    auto savePointTrigger = GameObject::Instantiate("SavePointTrigger", savePoint);
    savePointTrigger->AddComponent<BoxCollider>()->SetOffset({0, 0.5, 0});
    savePointTrigger->GetComponent<BoxCollider>()->SetSize({2, 2, 2});
    savePointTrigger->GetComponent<BoxCollider>()->isTrigger = true;
    savePointTrigger->AddComponent<SavePointTrigger>();

    return savePoint;
}

std::shared_ptr<StaticObjectData> SavePoint::SaveStatic() {
    std::shared_ptr<StaticObjectData> newData = std::make_shared<StaticObjectData>();
    std::shared_ptr<Renderer> objectRenderer = GetComponent<Renderer>();
    std::shared_ptr<BoxCollider> colider = GetComponent<BoxCollider>();

    newData->name = this->GetPrefabName();
    newData->uniqueName = this->GetName();
    newData->position = this->transform->GetLocalPosition();
    newData->rotation = this->transform->GetLocalRotation();
    newData->scale = this->transform->GetLocalScale();
    newData->modelPath = objectRenderer->lastLoadedModelPath;
    newData->coliderSize = colider->GetSize();
    newData->coliderOffset = colider->GetOffset();

    return newData;
}
