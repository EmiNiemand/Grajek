#ifndef GLOOMENGINE_OPTIONSMENU_H
#define GLOOMENGINE_OPTIONSMENU_H

#include <vector>
#include "Components/UI/Menu.h"
#include "glm/vec2.hpp"

class Button;

class OptionsMenu : public Menu {
private:
    short musicVolumeIterator = 9, windowResolutionIterator = 2, windowFullScreenIterator = 1, shadowResolutionIterator = 3;
    std::vector<std::string> musicVolumeValues = {"", "0.0", "0.125", "0.250", "0.375", "0.5", "0.625", "0.75", "0.875", "1.0", ""};
    std::vector<std::string> windowResolutionValues = {"", "960 x 540", "1440 x 810", "1920 x 1080", ""};
    std::vector<std::string> windowFullScreenValues = {"", "Off", "On", ""};
    std::vector<std::string> shadowResolutionValues = {"", "Low", "Medium", "High", ""};
    int musicVolumeButtonY[10] = {0, 175, 230, 285, 340, 395, 450, 500, 550, 600};
    int windowResolutionButtonY[4] = {0, 175, 395, 600};
    int windowFullScreenButtonY[3] = {0, 175, 600};
    int shadowResolutionButtonY[4] = {0, 175, 395, 600};

public:
    OptionsMenu(const std::shared_ptr<GameObject> &parent, int id);
    ~OptionsMenu() override;

    void ShowMenu();
    void ChangeActiveButton(glm::vec2 moveVector) override;
    void ChangeValue(float y);
    void OnClick() override;
};

#endif //GLOOMENGINE_OPTIONSMENU_H
