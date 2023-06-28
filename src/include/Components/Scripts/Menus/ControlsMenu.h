#ifndef GAME_CPP_CONTROLSMENU_H
#define GAME_CPP_CONTROLSMENU_H

#include "Components/UI/Menu.h"

class PlayerManager;

class ControlsMenu : public Menu {
    bool sameFrame = true;
public:
    ControlsMenu(const std::shared_ptr<GameObject> &parent, int id);
    ~ControlsMenu() override;

    void Start() override;
    void Update() override;
    bool ShowMenu() override;
};

#endif //GAME_CPP_CONTROLSMENU_H