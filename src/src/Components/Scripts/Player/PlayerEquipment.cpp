#include "Components/Scripts/Player/PlayerEquipment.h"
#include "Components/Scripts/Instrument.h"
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
    instruments.insert(instrument);

    return true;
}

std::shared_ptr<Instrument> PlayerEquipment::GetInstrumentWithName(InstrumentName name) {
    for (auto instrument: instruments) {
        if(instrument->name == name) return instrument;
    }
    return nullptr;
}

std::set<InstrumentName> PlayerEquipment::GetInstrumentNames() {
    std::set<InstrumentName> names;
    for(const auto& instrument : instruments) {
        names.insert(instrument->name);
    }
    return names;
}

void PlayerEquipment::AddReward(float crowdSatisfaction) {
    cash += (int)(crowdSatisfaction * maxCashReward);
    rep += (int)(crowdSatisfaction * maxRepReward);
}

int PlayerEquipment::GetCash() const { return cash; }

int PlayerEquipment::GetRep() const { return rep; }
