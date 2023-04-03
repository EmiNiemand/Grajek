#ifndef GLOOMENGINE_PLAYEREQUIPMENT_H
#define GLOOMENGINE_PLAYEREQUIPMENT_H


#include "Components/Component.h"
#include "Interfaces/IDataPersistance.h"
#include "Instrument.h"
#include <unordered_map>

class PlayerEquipment : public Component, public IDataPersistance {
private:
    int cash = 0;
    int rep = 0;
    std::unordered_map<std::shared_ptr<Instrument>, int> instruments;

public:
    PlayerEquipment(const std::shared_ptr<GameObject> &parent, int id);
    ~PlayerEquipment() override;

    void Setup();
    bool BuyInstrument(int price, const std::shared_ptr<Instrument>& instrument);

    void LoadData(std::shared_ptr<GameData> data) override;
    void SaveData(std::shared_ptr<GameData> &data) override;
};


#endif //GLOOMENGINE_PLAYEREQUIPMENT_H
