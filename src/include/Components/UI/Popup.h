#ifndef GAME_CPP_POPUP_H
#define GAME_CPP_POPUP_H

#include "Components/UI/Menu.h"

class Button;
class PlayerManager;

class Popup : public Menu {
private:
    std::shared_ptr<PlayerManager> playerManager;

public:
    Popup(const std::shared_ptr<GameObject> &parent, int id);
    ~Popup() override;

    void Setup(int x, int y, const std::string &imagePath, const std::string &buttonPathInactive, const std::string &buttonPathActive);
    void Update() override;
    void OnDestroy() override;
};

#endif //GAME_CPP_POPUP_H