//
// Created by masterktos on 03.04.23.
//

#ifndef GLOOMENGINE_PLAYERUI_H
#define GLOOMENGINE_PLAYERUI_H

#include "Components/Component.h"

class Text;

class PlayerUI : public Component {
private:
    std::shared_ptr<Text> cashText;
    std::shared_ptr<Text> repText;
public:
    PlayerUI(const std::shared_ptr<GameObject> &parent, int id);

    void UpdateCash(int newAmount);
    void UpdateRep(int newAmount);
};


#endif //GLOOMENGINE_PLAYERUI_H
