#include "GameObjectsAndPrefabs/Prefabs/SavePoint.h"
#include "Components/Renderers/Renderer.h"
#include "Components/PhysicsAndColliders/BoxCollider.h"
#include "Components/Scripts/Menus/SavePointTrigger.h"
#include "Components/UI/Image.h"
#include "Components/Animations/GameObjectAnimator.h"

SavePoint::SavePoint(const std::string &name, int id, const std::shared_ptr<GameObject> &parent, Tags tag) : Prefab(
        name, id, parent, tag) {
    prefabName = "SavePoint";
}

SavePoint::~SavePoint() = default;

std::shared_ptr<GameObject> SavePoint::Create() {
    auto savePoint = shared_from_this();
    savePoint->AddComponent<Renderer>()->LoadModel("SavePoint/SavePoint.obj");
    savePoint->AddComponent<BoxCollider>()->SetOffset({0, 0, 0});
    savePoint->GetComponent<BoxCollider>()->SetSize({0.5, 1, 0.5});
    std::shared_ptr<GameObject> s = GameObject::Instantiate("S", savePoint);
    s->AddComponent<Renderer>()->LoadModel("SavePoint/S.obj");
    s->AddComponent<GameObjectAnimator>()->Setup(s->transform, {
            {AnimatedProperty::Rotation, glm::vec3(0.0f, -360.0f, 0.0f), 5.0f},
    }, true);

    auto savePointTrigger = GameObject::Instantiate("SavePointTrigger", savePoint);
    savePointTrigger->AddComponent<BoxCollider>()->SetOffset({0, 0.5, 0});
    savePointTrigger->GetComponent<BoxCollider>()->SetSize({1, 1, 1});
    savePointTrigger->GetComponent<BoxCollider>()->isTrigger = true;
    savePointTrigger->AddComponent<SavePointTrigger>();
    GameObject::Instantiate("SavePointButton", savePointTrigger)->AddComponent<Image>()->LoadTexture(1600, 50, "UI/Sklep/Przycisk.png");
    savePointTrigger->children.begin()->second->DisableSelfAndChildren();

    return savePoint;
}

std::shared_ptr<StaticObjectData> SavePoint::SaveStatic() {
    std::shared_ptr<StaticObjectData> newData = std::make_shared<StaticObjectData>();
    std::shared_ptr<Renderer> objectRenderer = GetComponent<Renderer>();
    std::shared_ptr<BoxCollider> colider = GetComponent<BoxCollider>();

    newData->name = this->GetPrefabName();
    newData->position = this->transform->GetLocalPosition();
    newData->rotation = this->transform->GetLocalRotation();
    newData->scale = this->transform->GetLocalScale();
    newData->modelPath = objectRenderer->lastLoadedModelPath;
    newData->coliderSize = colider->GetSize();
    newData->coliderOffset = colider->GetOffset();

    return newData;
}
