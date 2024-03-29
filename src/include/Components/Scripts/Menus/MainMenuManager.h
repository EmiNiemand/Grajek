#ifndef GLOOMENGINE_MAINMENUMANAGER_H
#define GLOOMENGINE_MAINMENUMANAGER_H

#include "Components/Component.h"
#include "glm/vec2.hpp"

class Menu;
class MainMenu;
class LoadGameMenu;
class OptionsMenu;
class CreditsMenu;

class MainMenuManager : public Component {
public:
    std::shared_ptr<Menu> activeMenu;
    std::shared_ptr<MainMenu> mainMenu;
    std::shared_ptr<LoadGameMenu> loadGameMenu;
    std::shared_ptr<OptionsMenu> optionsMenu;
    std::shared_ptr<CreditsMenu> creditsMenu;
    bool inputEnabled = true;

public:
    void Start() override;
    void Update() override;
    void OnDestroy() override;

private:

    void PollInput();

    void OnMenuToggle();
    void OnApply() const;
    void OnUIMove(glm::vec2 moveVector) const;

public:
    MainMenuManager(const std::shared_ptr<GameObject> &parent, int id);
};

#endif //GLOOMENGINE_MAINMENUMANAGER_H