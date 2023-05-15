#ifndef GLOOMENGINE_NEWGAMEMENU_H
#define GLOOMENGINE_NEWGAMEMENU_H

#include <vector>
#include "Components/UI/Menu.h"

class NewGameMenu : public Menu {
public:
    std::string file;

public:
    NewGameMenu(const std::shared_ptr<GameObject> &parent, int id);
    ~NewGameMenu() override;

    void ShowMenu();
    void ChangeActiveButton(glm::vec2 moveVector) override;
    void OnClick() override;
};

#endif //GLOOMENGINE_NEWGAMEMENU_H