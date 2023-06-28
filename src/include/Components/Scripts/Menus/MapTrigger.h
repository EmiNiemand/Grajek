#ifndef GAME_CPP_MAPTRIGGER_H
#define GAME_CPP_MAPTRIGGER_H

#include "Components/Component.h"

class Image;
class PlayerManager;

class MapTrigger : public Component {
    std::shared_ptr<Image> mapImage;
    std::shared_ptr<PlayerManager> playerManager;

public:
    std::shared_ptr<Image> background;
    std::shared_ptr<Image> buttonImage;
    bool triggerActive = false;

public:
    MapTrigger(const std::shared_ptr<GameObject> &parent, int id);
    ~MapTrigger() override;

    void Start() override;
    void OnDestroy() override;
    void OnTriggerEnter(const std::shared_ptr<GameObject>& gameObject) override;
    void OnTriggerExit(const std::shared_ptr<GameObject>& gameObject) override;
    void Update() override;
};

#endif //GAME_CPP_MAPTRIGGER_H