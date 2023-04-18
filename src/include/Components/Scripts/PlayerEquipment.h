#ifndef GLOOMENGINE_PLAYEREQUIPMENT_H
#define GLOOMENGINE_PLAYEREQUIPMENT_H


#include "Components/Component.h"
#include "Interfaces/IDataPersistance.h"
#include "Instrument.h"
#include <unordered_map>

class PlayerEquipment : public Component, public IDataPersistance {
private:
	inline static int maxCashReward = 5;
	inline static int maxRepReward = 10;

public:
    std::unordered_map<std::shared_ptr<Instrument>, int> instruments;

    int cash = 0;
    int rep = 0;

    PlayerEquipment(const std::shared_ptr<GameObject> &parent, int id);
    ~PlayerEquipment() override;

    void Setup(int startCash=0, int startRep=0);
    bool BuyInstrument(int price, const std::shared_ptr<Instrument>& instrument);

    void AddReward(float crowdSatisfaction);

    void LoadData(std::shared_ptr<GameData> data) override;
    void SaveData(std::shared_ptr<GameData> &data) override;

    int GetCash() const;
    int GetRep() const;
};


#endif //GLOOMENGINE_PLAYEREQUIPMENT_H
