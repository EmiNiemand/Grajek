#ifndef GLOOMENGINE_SESSIONSTARTER_H
#define GLOOMENGINE_SESSIONSTARTER_H

#include "Components/UI/Menu.h"
#include "glm/vec2.hpp"
#include <set>
#include <vector>

class Button;
class Instrument;

class SessionStarter : public Menu {
private:
    std::vector<std::shared_ptr<Button>> buttons;

public:
    SessionStarter(const std::shared_ptr<GameObject> &parent, int id);
    ~SessionStarter() override;

    void Setup(const std::set<std::shared_ptr<Instrument>>& instruments);
    void ChangeActiveButton(glm::vec2 moveVector) override;
    void OnClick() override;
    void Stop();
};

#endif //GLOOMENGINE_SESSIONSTARTER_H
