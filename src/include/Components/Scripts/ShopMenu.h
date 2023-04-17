#ifndef GLOOMENGINE_SHOPMENU_H
#define GLOOMENGINE_SHOPMENU_H

#include "Components/Component.h"
#include "glm/vec2.hpp"

class Button;

class ShopMenu : public Component {
private:
    std::shared_ptr<Button> activeButton;

public:
    ShopMenu(const std::shared_ptr<GameObject> &parent, int id);
    ~ShopMenu() override;

    void Start() override;
    void ShowMenu();
    void HideMenu();
    void ChangeActiveButton(glm::vec2 moveVector);
    void OnClick();
};

#endif //GLOOMENGINE_SHOPMENU_H
