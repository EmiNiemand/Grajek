#ifndef GLOOMENGINE_SESSIONSTARTER_H
#define GLOOMENGINE_SESSIONSTARTER_H

#include "Components/UI/Menu.h"
#include "Components/Scripts/Instrument.h"
#include "glm/vec2.hpp"
#include <vector>

class Button;

class SessionStarter : public Menu {
private:
    std::vector<std::shared_ptr<Button>> buttons;

public:
    SessionStarter(const std::shared_ptr<GameObject> &parent, int id);
    ~SessionStarter() override;

    void Setup(std::unordered_map<std::shared_ptr<Instrument>, int> instruments);
    void OnClick();
    void Stop();
};

#endif //GLOOMENGINE_SESSIONSTARTER_H
