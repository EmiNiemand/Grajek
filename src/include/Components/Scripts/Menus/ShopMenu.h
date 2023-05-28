#ifndef GLOOMENGINE_SHOPMENU_H
#define GLOOMENGINE_SHOPMENU_H

#include <vector>
#include "Components/UI/Menu.h"
#include "glm/vec2.hpp"

class Button;

class ShopMenu : public Menu {
private:
    std::vector<std::shared_ptr<Button>> instruments;
    short secondInstrumentIndex = 0, thirdInstrumentIndex = 0, fourthInstrumentIndex = 0;

public:
    ShopMenu(const std::shared_ptr<GameObject> &parent, int id);
    ~ShopMenu() override;

    void Awake() override;
    void OnDestroy() override;

    void ChangeActiveButton(glm::vec2 moveVector) override;
    bool ShowMenu();
    void OnClick() override;
private:
    void DeleteButton(std::shared_ptr<Button> button);
};

#endif //GLOOMENGINE_SHOPMENU_H
