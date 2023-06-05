#ifndef GAME_CPP_CREDITSMENU_H
#define GAME_CPP_CREDITSMENU_H

#include "Components/UI/Menu.h"

class CreditsMenu : public Menu {
public:
    CreditsMenu(const std::shared_ptr<GameObject> &parent, int id);
    ~CreditsMenu() override;

    void ShowMenu();
};

#endif //GAME_CPP_CREDITSMENU_H