//
// Created by masterktos on 03.04.23.
//

#ifndef GLOOMENGINE_PLAYERUI_H
#define GLOOMENGINE_PLAYERUI_H

#include "Components/Component.h"
#include <map>

class Text;
class Image;

class PlayerUI : public Component {
private:
    std::shared_ptr<Text> cashText;
    std::shared_ptr<Image> cashBackground;
    std::map<PlayerBadges, std::shared_ptr<Image>> badges;
    int cashAmount;

public:
    PlayerUI(const std::shared_ptr<GameObject> &parent, int id);

    void OnDestroy() override;

    void UpdateCash(int newAmount, bool playAnimation=true);
    void UpdateBadges(const std::map<PlayerBadges, bool>& badgesStates);
};


#endif //GLOOMENGINE_PLAYERUI_H
