#ifndef IMAGE_CPP_SAVEPOINTMENU_H
#define IMAGE_CPP_SAVEPOINTMENU_H

#include "Components/UI/Menu.h"
#include "SavePointTrigger.h"
#include <vector>

class SavePointMenu : public Menu {
private:
    std::vector<std::shared_ptr<SavePointTrigger>> triggers;

public:
    SavePointMenu(const std::shared_ptr<GameObject> &parent, int id);
    ~SavePointMenu() override;

    void Awake() override;
    bool ShowMenu();
    void ChangeActiveButton(glm::vec2 moveVector) override;
    void OnClick() override;
};

#endif //IMAGE_CPP_SAVEPOINTMENU_H