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

    cashText->LoadFont("Cash", 10, 60, 20, glm::vec3(1.0f, 1.0f, 1.0f), GameFont::Eggnog);
    repText->LoadFont("Rep", 10, 100, 20, glm::vec3(1.0f, 1.0f, 1.0f), GameFont::Eggnog);
}

void PlayerUI::UpdateCash(int newAmount) {
    //TODO: update text
    //cashText.UpdateText("$$$: " + std::to_string(newAmount));
    spdlog::info("[PUI] Updated cash! [needs implementation]");
}

void PlayerUI::UpdateRep(int newAmount) {
    //TODO: update text
    //repText.UpdateText("Rep:  " + std::to_string(newAmount));
    spdlog::info("[PUI] Updated rep! [needs implementation]");
}