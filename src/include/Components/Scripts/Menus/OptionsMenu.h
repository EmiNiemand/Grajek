#ifndef GLOOMENGINE_OPTIONSMENU_H
#define GLOOMENGINE_OPTIONSMENU_H

#include <vector>
#include "Components/UI/Menu.h"
#include "glm/vec2.hpp"
#include <unordered_map>

class AudioSource;

class OptionsMenu : public Menu {
private:
    std::vector<std::string> musicVolumeValues = {"0.000", "0.125", "0.250", "0.375", "0.500", "0.625", "0.750", "0.875", "1.000"};
    std::vector<std::string> windowResolutionValues = {"960 x 540", "1440 x 810", "1920 x 1080"};
    std::vector<std::string> windowFullScreenValues = {"Off", "On"};
    std::vector<std::string> shadowResolutionValues = {"Low", "Medium", "High"};

    std::vector<std::shared_ptr<Button>> musicVolumeButtons;
    std::vector<std::shared_ptr<Button>> windowResolutionButtons;
    std::vector<std::shared_ptr<Button>> windowFullScreenButtons;
    std::vector<std::shared_ptr<Button>> shadowResolutionButtons;
    std::shared_ptr<Button> musicVolumeValue;
    std::shared_ptr<Button> windowResolutionValue;
    std::shared_ptr<Button> windowFullScreenValue;
    std::shared_ptr<Button> shadowResolutionValue;

    float previousMusicVolume, previousWindowResolutionWidth, previousWindowResolutionHeight, previousWindowFullScreen, previousShadowResolution;
    short musicVolumeIterator = 0, windowResolutionIterator = 0, windowFullScreenIterator = 0, shadowResolutionIterator = 0;

public:
    std::shared_ptr<GameObject> chooseMenu;

public:
    OptionsMenu(const std::shared_ptr<GameObject> &parent, int id);
    ~OptionsMenu() override;

    void Start() override;
    bool ShowMenu() override;
    void ChangeActiveButton(glm::vec2 moveVector) override;
    void ChangeValue(float x);
    void OnClick() override;
    static void ChangeShadowResolution();
    void CancelSettings();
    void OnDestroy() override;
};

#endif //GLOOMENGINE_OPTIONSMENU_H
