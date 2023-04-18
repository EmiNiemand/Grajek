#ifndef GLOOMENGINE_SESSIONSTARTER_H
#define GLOOMENGINE_SESSIONSTARTER_H

#include "Components/Component.h"
#include "glm/vec2.hpp"

class Button;

class SessionStarter : public Component {
private:
    std::shared_ptr<Button> activeButton;

public:
    SessionStarter(const std::shared_ptr<GameObject> &parent, int id);
    ~SessionStarter() override;

    void Start() override;
    void ShowMenu();
    void HideMenu();
    void ChangeActiveButton(glm::vec2 moveVector);
    void OnClick();
};

#endif //GLOOMENGINE_SESSIONSTARTER_H
