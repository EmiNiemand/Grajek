//
// Created by masterktos on 03.04.23.
//

#include "Components/Scripts/PlayerUI.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/UI/Text.h"
#include "Utilities.h"

PlayerUI::PlayerUI(const std::shared_ptr<GameObject> &parent, uint32_t id)
        : Component(parent, id) {
    cashText = GameObject::Instantiate("CashText", parent)->AddComponent<Text>();
    repText = GameObject::Instantiate("RepText", parent)->AddComponent<Text>();

    cashText->LoadFont("$$$: -", 10, 330, 30);
    repText->LoadFont("Rep:  -", 10, 300, 30);
}

void PlayerUI::UpdateCash(int newAmount) {
	cashText->text = "$$$: " + std::to_string(newAmount);
}

void PlayerUI::UpdateRep(int newAmount) {
	repText->text = "Rep:  " + std::to_string(newAmount);
}