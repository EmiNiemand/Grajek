//
// Created by szymo on 21/06/2023.
//

#include "GameObjectsAndPrefabs/Prefabs/Crowd/Default.h"
#include "Components/PhysicsAndColliders/BoxCollider.h"
#include "Components/Renderers/Animator.h"
#include "EngineManagers/RandomnessManager.h"

namespace Crowd {
    Default::Default(const std::string &name, int id, const std::shared_ptr<GameObject> &parent, Tags tag) : Prefab(
            name, id, parent, tag) {
        prefabName = "CrowdDefaultCharacter";
    }

    Default::~Default() = default;

    std::shared_ptr<GameObject> Default::Create() {
        auto character = shared_from_this();
        // Setting values
        auto collider = character->AddComponent<BoxCollider>();
        collider->SetOffset({0, 1.5, 0.1});
        collider->SetSize({0.5, 2, 0.6});
        character->transform->SetLocalScale({0.5, 0.5, 0.5});

        auto animator = character->AddComponent<Animator>();

        int modelIndex = RandomnessManager::GetInstance()->GetInt(1, 6);

        std::string model;

        if (modelIndex < 10)
            model = "BasicMan00" + std::to_string(modelIndex);
        else
            model = "BasicMan0" + std::to_string(modelIndex);

        animator->LoadAnimationModel("Crowd/" + model + "/" + model + ".dae");


        modelIndex = RandomnessManager::GetInstance()->GetInt(1, 3);

        switch(modelIndex) {
            case 1:
                animator->SetAnimation("CrowdAnimations/Idle1.dae");
                break;
            case 2:
                animator->SetAnimation("CrowdAnimations/Idle3.dae");
                break;
            case 3:
                animator->SetAnimation("CrowdAnimations/Happy.dae");
                break;
            default:
                break;
        }

        return character;
    }

} // Crowd