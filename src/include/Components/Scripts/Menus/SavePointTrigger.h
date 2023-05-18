#ifndef IMAGE_CPP_SAVEPOINTTRIGGER_H
#define IMAGE_CPP_SAVEPOINTTRIGGER_H

#include "Components/Component.h"

class SavePointTrigger : public Component {
public:
    bool active = false;

public:
    SavePointTrigger(const std::shared_ptr<GameObject> &parent, int id);
    ~SavePointTrigger() override;

    void OnTriggerStay(const std::shared_ptr<GameObject>& gameObject) override;
    void OnTriggerExit(const std::shared_ptr<GameObject>& gameObject) override;
};

#endif //IMAGE_CPP_SAVEPOINTTRIGGER_H