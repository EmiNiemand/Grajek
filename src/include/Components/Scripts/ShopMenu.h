#ifndef GLOOMENGINE_SHOPMENU_H
#define GLOOMENGINE_SHOPMENU_H

#include "Components/UI/Menu.h"
#include "glm/vec2.hpp"

class Button;

class ShopMenu : public Menu {
public:
    ShopMenu(const std::shared_ptr<GameObject> &parent, int id);
    ~ShopMenu() override;

    void Start() override;

    bool ShowMenu();
    void OnClick() override;
private:
    void DeleteButton(std::shared_ptr<Button> button);
};

#endif //GLOOMENGINE_SHOPMENU_H
