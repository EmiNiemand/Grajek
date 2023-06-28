#ifndef GAME_CPP_CONTROLSMENU_H
#define GAME_CPP_CONTROLSMENU_H

#include "Components/UI/Menu.h"

class PlayerManager;

class ControlsMenu : public Menu {
public:
    ControlsMenu(const std::shared_ptr<GameObject> &parent, int id);
    ~ControlsMenu() override;

    void Start() override;
    bool ShowMenu() override;
    void OnClick() override;
};

#endif //GAME_CPP_CONTROLSMENU_H