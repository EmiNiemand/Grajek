#ifndef GLOOMENGINE_OPTIONSMENU_H
#define GLOOMENGINE_OPTIONSMENU_H

#include "Components/UI/Menu.h"
#include "glm/vec2.hpp"

class Button;

class OptionsMenu : public Menu {
public:
    OptionsMenu(const std::shared_ptr<GameObject> &parent, int id);
    ~OptionsMenu() override;

    void ShowMenu();
    void OnClick();
};

#endif //GLOOMENGINE_OPTIONSMENU_H
