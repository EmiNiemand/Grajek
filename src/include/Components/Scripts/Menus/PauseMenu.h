#ifndef GLOOMENGINE_PAUSEMENU_H
#define GLOOMENGINE_PAUSEMENU_H

#include "Components/UI/Menu.h"
#include "glm/vec2.hpp"

class Button;
class ControlsMenu;

class PauseMenu : public Menu {
    std::shared_ptr<ControlsMenu> controlsMenu;

public:
    bool gameShouldExit = false;

    PauseMenu(const std::shared_ptr<GameObject> &parent, int id);
    ~PauseMenu() override;

    void Start() override;
    bool ShowMenu() override;
    void OnClick() override;

    void HideMenu() override;
};

#endif //GLOOMENGINE_PAUSEMENU_H