//
// Created by masterktos on 03.04.23.
//

#include "Components/Scripts/PlayerUI.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Utilities.h"

PlayerUI::PlayerUI(const std::shared_ptr<GameObject> &parent, int id)
        : Component(parent, id) {
    cashText = GameObject::Instantiate("CashText", parent)->AddComponent<Text>();
    repText = GameObject::Instantiate("RepText", parent)->AddComponent<Text>();

    cashText->LoadFont("Cash: 5", 10, 330, 30, glm::vec3(0), GameFont::Eggnog);
    repText->LoadFont("Rep:  5", 10, 300, 30, glm::vec3(0), GameFont::Eggnog);
}

void PlayerUI::UpdateCash(int newAmount) {
    //TODO: update text
    //cashText.UpdateText("Cash: " + std::to_string(newAmount));
    spdlog::info("[PUI] Updated cash! [needs implementation]");
}

void PlayerUI::UpdateRep(int newAmount) {
    //TODO: update text
    //repText.UpdateText("Rep:  " + std::to_string(newAmount));
    spdlog::info("[PUI] Updated rep! [needs implementation]");
}