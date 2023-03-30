//
// Created by masterktos on 30.03.23.
//

#ifndef GLOOMENGINE_PLAYEREQUIPMENT_H
#define GLOOMENGINE_PLAYEREQUIPMENT_H


#include "Components/Component.h"
#include "Instrument.h"

class PlayerEquipment : Component {
private:
    int cash;
    int rep;

public:
    void Setup();
    void BuyInstrument(int price, Instrument instrument);
};


#endif //GLOOMENGINE_PLAYEREQUIPMENT_H
