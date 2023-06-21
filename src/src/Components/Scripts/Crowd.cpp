//
// Created by szymo on 21/06/2023.
//

#include "Components/Scripts/Crowd.h"
#include "GameObjectsAndPrefabs/Prefabs/Crowd/Default.h"
#include "GameObjectsAndPrefabs/Prefabs/Crowd/Jazz.h"
#include "Components/Scripts/Player/PlayerEquipment.h"
#include "EngineManagers/RandomnessManager.h"

namespace Crowd {
    Crowd::Crowd(const std::shared_ptr <GameObject> &parent, int id) : Component(parent, id) {}

    Crowd::~Crowd() = default;

    void Crowd::Start() {
        if (!GloomEngine::GetInstance()->FindGameObjectWithName("Player")->GetComponent<PlayerEquipment>()->badges[PlayerBadges::DRUMS]) {
            auto jazzusPosition = GloomEngine::GetInstance()->FindGameObjectWithName("JazzOpponent")->transform->GetGlobalPosition();

            jazzCrowd = GameObject::Instantiate("JazzCrowd", SceneManager::GetInstance()->activeScene);

            for (int i = 0; i < 30; ++i) {
                float randomNumber = RandomnessManager::GetInstance()->GetFloat(1, 1.25f);
                int prefabNumber = RandomnessManager::GetInstance()->GetInt(1, 10);

                std::shared_ptr<GameObject> crowdMember;

                if(prefabNumber < 4) crowdMember = Prefab::Instantiate<Default>("CrowdMember");
                else crowdMember = Prefab::Instantiate<Jazz>("CrowdMember");

                crowdMember->SetParent(jazzCrowd);
                crowdMember->transform->SetLocalPosition(glm::vec3(
                        std::sin(glm::radians((float)i * 12.0f)) * 3.25 * randomNumber + jazzusPosition.x, jazzusPosition.y,
                        std::cos(glm::radians((float)i * 12.0f)) * 3.25 * randomNumber + jazzusPosition.z));
                crowdMember->transform->SetLocalRotation(glm::vec3(0, 180 + 12 * i, 0));
            }
        }
        Component::Start();
    }

    void Crowd::OnEnemyDefeat(PlayerBadges badge) {
        if (badge == PlayerBadges::DRUMS) {
            GameObject::Destroy(jazzCrowd);
        }
    }
}


