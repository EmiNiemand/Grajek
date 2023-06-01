#ifndef GAME_CPP_OPPONENTSESSIONSTARTER_H
#define GAME_CPP_OPPONENTSESSIONSTARTER_H

#include "Components/UI/Menu.h"
#include "glm/vec2.hpp"
#include <set>
#include <vector>

class Button;
class Instrument;

class OpponentSessionStarter : public Menu {
private:
    std::vector<std::shared_ptr<Button>> buttons;

public:
    OpponentSessionStarter(const std::shared_ptr<GameObject> &parent, int id);
    ~OpponentSessionStarter() override;

    void OnDestroy() override;

    void Setup(const std::set<std::shared_ptr<Instrument>>& instruments);
    void ChangeActiveButton(glm::vec2 moveVector) override;
    void OnClick() override;
};

#endif //GAME_CPP_OPPONENTSESSIONSTARTER_H