#include "Components/Scripts/PlayerEquipment.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "LowLevelClasses/GameData.h"

PlayerEquipment::PlayerEquipment(const std::shared_ptr<GameObject> &parent, int id) : Component(parent, id) {}

PlayerEquipment::~PlayerEquipment() = default;

void PlayerEquipment::Setup(int startCash, int startRep) {
    cash = startCash;
    rep = startRep;
}

bool PlayerEquipment::BuyInstrument(int price, const std::shared_ptr<Instrument>& instrument) {
    if(price > cash || instruments.contains(instrument))
        return false;

    cash -= price;
    instruments.insert({instrument, 0});

    return true;
}

void PlayerEquipment::AddReward(float crowdSatisfaction) {
    cash += (int)(crowdSatisfaction * maxCashReward);
    rep += (int)(crowdSatisfaction * maxRepReward);
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

int PlayerEquipment::GetCash() const { return cash; }

int PlayerEquipment::GetRep() const { return rep; }
