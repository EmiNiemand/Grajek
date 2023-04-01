#include "Components/Scripts/PlayerEquipment.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "LowLevelClasses/GameData.h"

PlayerEquipment::PlayerEquipment(const std::shared_ptr<GameObject> &parent, int id) : Component(parent, id) {}

PlayerEquipment::~PlayerEquipment() = default;

void PlayerEquipment::Setup() {

}

void PlayerEquipment::BuyInstrument(int price, Instrument instrument) {

}

void PlayerEquipment::LoadData(std::shared_ptr<GameData> data) {
    cash = data->money;
    rep = data->reputation;
    parent->transform->SetLocalPosition(data->playerPosition);
}

void PlayerEquipment::SaveData(std::shared_ptr<GameData> &data) {
    data->money = cash;
    data->reputation = rep;
    data->playerPosition = parent->transform->GetLocalPosition();
}
