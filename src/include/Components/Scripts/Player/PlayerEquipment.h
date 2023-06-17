#ifndef GLOOMENGINE_PLAYEREQUIPMENT_H
#define GLOOMENGINE_PLAYEREQUIPMENT_H


#include "Components/Component.h"
#include "Interfaces/IDataPersistance.h"
#include "Components/Scripts/MusicPattern.h"
#include <set>
#include <map>

class Instrument;

class PlayerEquipment : public Component {
private:
	inline static float maxCashReward = 10.0f;

public:
    std::set<std::shared_ptr<Instrument>> instruments;
    std::map<PlayerBadges, bool> badges;

    int cash = 0;

    PlayerEquipment(const std::shared_ptr<GameObject> &parent, int id);
    ~PlayerEquipment() override;

    void OnDestroy() override;

    bool BuyInstrument(int price, const std::shared_ptr<Instrument>& instrument);
    std::shared_ptr<Instrument> GetInstrumentWithName(InstrumentName name);
    std::set<InstrumentName> GetInstrumentNames();

    void AddReward(float crowdSatisfaction);

    int GetCash() const;
};


#endif //GLOOMENGINE_PLAYEREQUIPMENT_H
