#ifndef GLOOMENGINE_PAUSEMENU_H
#define GLOOMENGINE_PAUSEMENU_H

#include "Components/UI/Menu.h"
#include "glm/vec2.hpp"

class Button;

class PauseMenu : public Menu {
public:
    PauseMenu(const std::shared_ptr<GameObject> &parent, int id);
    ~PauseMenu() override;

    void ShowMenu();
    void OnClick();
};

#endif //GLOOMENGINE_PAUSEMENU_H