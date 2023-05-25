#ifndef IMAGE_CPP_SAVEPOINTTRIGGER_H
#define IMAGE_CPP_SAVEPOINTTRIGGER_H

#include "Components/Component.h"

class Image;

class SavePointTrigger : public Component {
public:
    bool active = false;
    std::shared_ptr<Image> buttonImage;

public:
    SavePointTrigger(const std::shared_ptr<GameObject> &parent, int id);
    ~SavePointTrigger() override;

    void Start() override;
    void OnTriggerEnter(const std::shared_ptr<GameObject>& gameObject) override;
    void OnTriggerExit(const std::shared_ptr<GameObject>& gameObject) override;
};

#endif //IMAGE_CPP_SAVEPOINTTRIGGER_H