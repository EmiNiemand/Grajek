#ifndef GAME_CPP_MAINMENUOPTIONSMENU_H
#define GAME_CPP_MAINMENUOPTIONSMENU_H

#include "Components/UI/Menu.h"
#include <vector>

class AudioSource;

class MainMenuOptionsMenu : public Menu {
private:
    short musicVolumeIterator = 0, windowResolutionIterator = 0, windowFullScreenIterator = 0, shadowResolutionIterator = 0;
    std::vector<std::string> musicVolumeValues = {"0.000", "0.125", "0.250", "0.375", "0.500", "0.625", "0.750", "0.875", "1.000"};
    std::vector<std::string> windowResolutionValues = {"960 x 540", "1440 x 810", "1920 x 1080"};
    std::vector<std::string> windowFullScreenValues = {"Off", "On"};
    std::vector<std::string> shadowResolutionValues = {"Low", "Medium", "High"};

    std::shared_ptr<Button> musicVolumeValue;
    std::shared_ptr<Button> windowResolutionValue;
    std::shared_ptr<Button> windowFullScreenValue;
    std::shared_ptr<Button> shadowResolutionValue;
    std::vector<std::shared_ptr<Button>> musicVolumeButtons;
    std::vector<std::shared_ptr<Button>> windowResolutionButtons;
    std::vector<std::shared_ptr<Button>> windowFullScreenButtons;
    std::vector<std::shared_ptr<Button>> shadowResolutionButtons;
    std::shared_ptr<AudioSource> sound;

public:
    MainMenuOptionsMenu(const std::shared_ptr<GameObject> &parent, int id);
    ~MainMenuOptionsMenu() override;

    void Start() override;
    bool ShowMenu() override;
    void ChangeActiveButton(glm::vec2 moveVector) override;
    void OnClick() override;
    void ChangeValue(float x);
    static void ChangeShadowResolution();
    void OnDestroy() override;
};

#endif //GAME_CPP_MAINMENUOPTIONSMENU_H