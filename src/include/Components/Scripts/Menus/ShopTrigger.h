#ifndef GLOOMENGINE_SHOPTRIGGER_H
#define GLOOMENGINE_SHOPTRIGGER_H

#include "Components/Component.h"

class Image;
class ShopMenu;

class ShopTrigger : public Component {
private:
    std::shared_ptr<GameObject> door;

public:
    std::shared_ptr<ShopMenu> shopMenu;
    std::shared_ptr<Image> buttonImage;

public:
    ShopTrigger(const std::shared_ptr<GameObject> &parent, int id);
    ~ShopTrigger() override;

    void Start() override;
    void OnTriggerEnter(const std::shared_ptr<GameObject>& gameObject) override;
    void OnTriggerExit(const std::shared_ptr<GameObject>& gameObject) override;
};

#endif //GLOOMENGINE_SHOPTRIGGER_H
