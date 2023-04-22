#ifndef GLOOMENGINE_SHOPMENU_H
#define GLOOMENGINE_SHOPMENU_H

#include "Components/UI/Menu.h"
#include "glm/vec2.hpp"

class Button;

class ShopMenu : public Menu {
public:
    ShopMenu(const std::shared_ptr<GameObject> &parent, int id);
    ~ShopMenu() override;

    void ShowMenu();
    void OnClick();
};

#endif //GLOOMENGINE_SHOPMENU_H
