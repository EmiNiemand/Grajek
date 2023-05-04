#ifndef GLOOMENGINE_SHOPTRIGGER_H
#define GLOOMENGINE_SHOPTRIGGER_H

#include "Components/Component.h"

class ShopTrigger : public Component {
public:
    bool active = false;

public:
    ShopTrigger(const std::shared_ptr<GameObject> &parent, uint32_t id);
    ~ShopTrigger() override;

    void OnTriggerEnter(const std::shared_ptr<GameObject>& gameObject) override;
    void OnTriggerExit(const std::shared_ptr<GameObject>& gameObject) override;
};

#endif //GLOOMENGINE_SHOPTRIGGER_H
