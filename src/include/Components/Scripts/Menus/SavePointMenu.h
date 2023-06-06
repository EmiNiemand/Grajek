#ifndef IMAGE_CPP_SAVEPOINTMENU_H
#define IMAGE_CPP_SAVEPOINTMENU_H

#include "Components/UI/Menu.h"
#include "SavePointTrigger.h"
#include "Interfaces/IDataPersistance.h"
#include <vector>

class SavePointMenu : public Menu, public IDataPersistance {
private:
    std::vector<std::shared_ptr<SavePointTrigger>> triggers;
    std::string saveDate;

public:
    bool triggerActive = false;
    std::shared_ptr<Image> buttonImage;
    std::vector<std::shared_ptr<Text>> saveDates;

public:
    SavePointMenu(const std::shared_ptr<GameObject> &parent, int id);
    ~SavePointMenu() override;

    void OnDestroy() override;

    bool ShowMenu() override;
    void HideMenu() override;
    void ChangeActiveButton(glm::vec2 moveVector) override;
    void OnClick() override;

    void LoadData(std::shared_ptr<GameData> data) override{};
    void SaveData(std::shared_ptr<GameData> &data) override;
};

#endif //IMAGE_CPP_SAVEPOINTMENU_H