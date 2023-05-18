//
// Created by masterktos on 03.04.23.
//

#include "Components/Scripts/Player/PlayerUI.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/UI/Text.h"
#include "Utilities.h"

PlayerUI::PlayerUI(const std::shared_ptr<GameObject> &parent, int id)
        : Component(parent, id) {
    cashText = GloomEngine::GetInstance()->FindGameObjectWithName("Money")->GetComponent<Text>();
    repText = GloomEngine::GetInstance()->FindGameObjectWithName("Reputation")->GetComponent<Text>();
}

void PlayerUI::UpdateCash(int newAmount) {
	cashText->text = "Money: " + std::to_string(newAmount);
}

void PlayerUI::UpdateRep(int newAmount) {
	repText->text = "Rep: " + std::to_string(newAmount);
}