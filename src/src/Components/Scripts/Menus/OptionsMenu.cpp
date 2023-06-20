#include "Components/Scripts/Menus/OptionsMenu.h"
#include "GloomEngine.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/UI/Button.h"
#include "Components/UI/Image.h"
#include "Components/Scripts/Menus/PauseMenu.h"
#include "EngineManagers/PostProcessingManager.h"
#include "Components/Audio/AudioListener.h"
#include "Components/Scripts/Player/PlayerManager.h"
#include "EngineManagers/ShadowManager.h"
#include "EngineManagers/RendererManager.h"
#include "EngineManagers/OptionsManager.h"
#include "Components/Scripts/Menus/MainMenuManager.h"
#include "Components/Scripts/Menus/MainMenu.h"
#include "EngineManagers/SceneManager.h"
#include "EngineManagers/HIDManager.h"
#include "EngineManagers/AudioManager.h"

OptionsMenu::OptionsMenu(const std::shared_ptr<GameObject> &parent, int id) : Menu(parent, id) {
    auto optionsManager = OptionsManager::GetInstance();
    musicVolumeIterator = (short)(optionsManager->musicVolume / 0.125f);
    windowResolutionIterator = (short)(optionsManager->width / 480 - 2);
    windowFullScreenIterator = (short)(optionsManager->fullScreen);
    shadowResolutionIterator = (short)(optionsManager->shadowResolution / 2048);
    AudioManager::GetInstance()->audioListener->SetGain(OptionsManager::GetInstance()->musicVolume);
    if (optionsManager->fullScreen) {
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        glfwSetWindowMonitor(GloomEngine::GetInstance()->window, monitor,
							 0, 0, mode->width, mode->height, mode->refreshRate);
    } else {
        const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        glfwSetWindowMonitor(GloomEngine::GetInstance()->window, NULL,
                             mode->width / 2 - optionsManager->width / 2,
                             mode->height / 2 - optionsManager->height / 2,
                             optionsManager->width,
                             optionsManager->height, 0);
    }
    ShadowManager::GetInstance()->shadowResolution = optionsManager->shadowResolution;
    ChangeShadowResolution();
}

OptionsMenu::~OptionsMenu() = default;

void OptionsMenu::Start() {
    std::string mainMenu;
    if (SceneManager::GetInstance()->activeScene->GetName() == "MainMenuScene")
        mainMenu = "MainMenu";
    musicVolumeValue = GloomEngine::GetInstance()->FindGameObjectWithName(mainMenu + "MusicVolumeValue")->GetComponent<Button>();
    windowResolutionValue = GloomEngine::GetInstance()->FindGameObjectWithName(mainMenu + "WindowResolutionValue")->GetComponent<Button>();
    windowFullScreenValue = GloomEngine::GetInstance()->FindGameObjectWithName(mainMenu + "WindowFullScreenValue")->GetComponent<Button>();
    shadowResolutionValue = GloomEngine::GetInstance()->FindGameObjectWithName(mainMenu + "ShadowResolutionValue")->GetComponent<Button>();
    musicVolumeValue->ChangeText(musicVolumeValues[musicVolumeIterator]);
    windowResolutionValue->ChangeText(windowResolutionValues[windowResolutionIterator]);
    windowFullScreenValue->ChangeText(windowFullScreenValues[windowFullScreenIterator]);
    shadowResolutionValue->ChangeText(shadowResolutionValues[shadowResolutionIterator]);
    musicVolumeButtons.push_back(GloomEngine::GetInstance()->FindGameObjectWithName(mainMenu + "MusicVolumeLeft")->GetComponent<Button>());
    musicVolumeButtons.push_back(GloomEngine::GetInstance()->FindGameObjectWithName(mainMenu + "MusicVolumeRight")->GetComponent<Button>());
    windowResolutionButtons.push_back(GloomEngine::GetInstance()->FindGameObjectWithName(mainMenu + "WindowResolutionLeft")->GetComponent<Button>());
    windowResolutionButtons.push_back(GloomEngine::GetInstance()->FindGameObjectWithName(mainMenu + "WindowResolutionRight")->GetComponent<Button>());
    windowFullScreenButtons.push_back(GloomEngine::GetInstance()->FindGameObjectWithName(mainMenu + "WindowFullScreenLeft")->GetComponent<Button>());
    windowFullScreenButtons.push_back(GloomEngine::GetInstance()->FindGameObjectWithName(mainMenu + "WindowFullScreenRight")->GetComponent<Button>());
    shadowResolutionButtons.push_back(GloomEngine::GetInstance()->FindGameObjectWithName(mainMenu + "ShadowResolutionLeft")->GetComponent<Button>());
    shadowResolutionButtons.push_back(GloomEngine::GetInstance()->FindGameObjectWithName(mainMenu + "ShadowResolutionRight")->GetComponent<Button>());

    if (musicVolumeIterator == 0) musicVolumeButtons[0]->isActive = false;
    if (musicVolumeIterator == 8) musicVolumeButtons[1]->isActive = false;
    if (windowResolutionIterator == 0) windowResolutionButtons[0]->isActive = false;
    if (windowResolutionIterator == 2) windowResolutionButtons[1]->isActive = false;
    if (windowFullScreenIterator == 0) windowFullScreenButtons[0]->isActive = false;
    if (windowFullScreenIterator == 1) windowFullScreenButtons[1]->isActive = false;
    if (shadowResolutionIterator == 0) shadowResolutionButtons[0]->isActive = false;
    if (shadowResolutionIterator == 2) shadowResolutionButtons[1]->isActive = false;
    sound = parent->AddComponent<AudioSource>();
    sound->LoadAudioData("res/sounds/direct/options_scroll.wav", AudioType::Direct);

    Component::Start();
}

bool OptionsMenu::ShowMenu() {
    if(!Menu::ShowMenu()) return false;

    if (SceneManager::GetInstance()->activeScene->GetName() == "MainMenuScene")
        activeButton = GloomEngine::GetInstance()->FindGameObjectWithName("MainMenuWindowResolutionButton")->GetComponent<Button>();
    else
        activeButton = GloomEngine::GetInstance()->FindGameObjectWithName("WindowResolutionButton")->GetComponent<Button>();
    activeButton->isActive = true;
    auto optionsManager = OptionsManager::GetInstance();
    previousMusicVolume = optionsManager->musicVolume;
    previousWindowResolutionWidth = (float)optionsManager->width;
    previousWindowResolutionHeight = (float)optionsManager->height;
    previousWindowFullScreen = optionsManager->fullScreen;
    previousShadowResolution = (float)optionsManager->shadowResolution;
    chooseMenu->DisableSelfAndChildren();
    if (SceneManager::GetInstance()->activeScene->GetName() == "Scene")
        SceneManager::GetInstance()->activeScene->GetComponent<AudioSource>()->SetGain(0);
    return true;
}

void OptionsMenu::ChangeActiveButton(glm::vec2 moveVector) {
    Menu::ChangeActiveButton(moveVector);
    if (moveVector.x != 0.0f)
        ChangeValue(moveVector.x);
}

void OptionsMenu::OnClick() {
    std::string mainMenu;
    if (SceneManager::GetInstance()->activeScene->GetName() == "MainMenuScene")
        mainMenu = "MainMenu";

    if (activeButton->GetParent()->GetName() == mainMenu + "SaveButton") {
        OptionsManager::GetInstance()->Save();
        if (GloomEngine::GetInstance()->FindGameObjectWithName("Player")) {
            GloomEngine::GetInstance()->FindGameObjectWithName(
                    "Player")->GetComponent<PlayerManager>()->ToggleOptionsMenu();
        } else {
            auto mainMenuManager = GloomEngine::GetInstance()->FindGameObjectWithName("MainMenuManager")->GetComponent<MainMenuManager>();
            HideMenu();
            mainMenuManager->activeMenu = mainMenuManager->mainMenu;
            mainMenuManager->mainMenu->ShowMenu();
        }
    }
    if (activeButton->GetParent()->GetName() == mainMenu + "CancelButton") {
        CancelSettings();
        if (GloomEngine::GetInstance()->FindGameObjectWithName("Player")) {
            GloomEngine::GetInstance()->FindGameObjectWithName(
                    "Player")->GetComponent<PlayerManager>()->ToggleOptionsMenu();
        } else {
            auto mainMenuManager = GloomEngine::GetInstance()->FindGameObjectWithName("MainMenuManager")->GetComponent<MainMenuManager>();
            HideMenu();
            mainMenuManager->activeMenu = mainMenuManager->mainMenu;
            mainMenuManager->mainMenu->ShowMenu();
        }
    }
    Menu::OnClick();
}

void OptionsMenu::ChangeValue(float x) {
    auto activeButtonName = activeButton->GetParent()->GetName();
    auto optionManager = OptionsManager::GetInstance();
    std::string mainMenu = "";
    if (SceneManager::GetInstance()->activeScene->GetName() == "MainMenuScene")
        mainMenu = "MainMenu";

    if (activeButtonName == mainMenu + "MusicVolumeButton") {
        std::shared_ptr<AudioListener> audioListener;
        std::shared_ptr<AudioSource> mainMenuMusic;
        audioListener = AudioManager::GetInstance()->audioListener;
        float gain = audioListener->GetGain();
        if (x == -1.0f) {
            if (optionManager->musicVolume <= 0.0f) return;
            if (audioListener)
                audioListener->SetGain(gain - 0.125f);
            optionManager->musicVolume -= 0.125f;
            musicVolumeIterator--;
        } else {
            if (optionManager->musicVolume >= 1.0f) return;
            if (audioListener)
                audioListener->SetGain(gain + 0.125f);
            optionManager->musicVolume += 0.125f;
            musicVolumeIterator++;
        }
        musicVolumeButtons[0]->isActive = true;
        musicVolumeButtons[1]->isActive = true;
        if (musicVolumeIterator == 0) musicVolumeButtons[0]->isActive = false;
        if (musicVolumeIterator == 8) musicVolumeButtons[1]->isActive = false;
        musicVolumeValue->ChangeText(musicVolumeValues[musicVolumeIterator]);
        sound->ForcePlaySound();
    } else if (activeButtonName == mainMenu + "WindowResolutionButton") {
        const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        if (x == -1.0f) {
            if (optionManager->width == 1920) {
                optionManager->width = 1440;
                optionManager->height = 810;
                glfwSetWindowPos(GloomEngine::GetInstance()->window, mode->width / 2 - optionManager->width / 2, mode->height / 2 - optionManager->height / 2);
                PostProcessingManager::GetInstance()->WindowResize();
            } else if (optionManager->width == 1440) {
                optionManager->width = 960;
                optionManager->height = 540;
                glfwSetWindowPos(GloomEngine::GetInstance()->window, mode->width / 2 - optionManager->width / 2, mode->height / 2 - optionManager->height / 2);
                PostProcessingManager::GetInstance()->WindowResize();
            } else return;
            windowResolutionIterator--;
        } else {
            if (optionManager->width == 960) {
                optionManager->width = 1440;
                optionManager->height = 810;
                glfwSetWindowPos(GloomEngine::GetInstance()->window, mode->width / 2 - optionManager->width / 2, mode->height / 2 - optionManager->height / 2);
                PostProcessingManager::GetInstance()->WindowResize();
            } else if (optionManager->width == 1440) {
                optionManager->width = 1920;
                optionManager->height = 1080;
                glfwSetWindowPos(GloomEngine::GetInstance()->window, mode->width / 2 - optionManager->width / 2, mode->height / 2 - optionManager->height / 2);
                PostProcessingManager::GetInstance()->WindowResize();
            } else return;
            windowResolutionIterator++;
        }
        if (windowFullScreenIterator == 1) {
            auto gloomEngine = GloomEngine::GetInstance();
            glfwSetWindowMonitor(gloomEngine->window, NULL,
                                 mode->width / 2 - optionManager->width / 2,
                                 mode->height / 2 - optionManager->height / 2,
                                 optionManager->width,
                                 optionManager->height, 0);
            windowFullScreenIterator--;
            optionManager->fullScreen = false;
            windowFullScreenButtons[0]->isActive = true;
            windowFullScreenButtons[1]->isActive = true;
            if (windowFullScreenIterator == 0) windowFullScreenButtons[0]->isActive = false;
            if (windowFullScreenIterator == 1) windowFullScreenButtons[1]->isActive = false;
            windowFullScreenValue->ChangeText(windowFullScreenValues[windowFullScreenIterator]);
        }
        windowResolutionButtons[0]->isActive = true;
        windowResolutionButtons[1]->isActive = true;
        if (windowResolutionIterator == 0) windowResolutionButtons[0]->isActive = false;
        if (windowResolutionIterator == 2) windowResolutionButtons[1]->isActive = false;
        PostProcessingManager::GetInstance()->WindowResize();
        windowResolutionValue->ChangeText(windowResolutionValues[windowResolutionIterator]);
        sound->ForcePlaySound();
    }

	else if (activeButtonName == mainMenu + "WindowFullScreenButton") {
        const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        if (x == -1.0f) {
            if (windowFullScreenIterator == 0) return;
            glfwSetWindowMonitor(GloomEngine::GetInstance()->window, NULL,
                                 mode->width / 2 - optionManager->width / 2,
                                 mode->height / 2 - optionManager->height / 2,
                                 optionManager->width,
                                 optionManager->height, 0);
            windowFullScreenIterator--;
            optionManager->fullScreen = false;
        } else {
            if (windowFullScreenIterator == 1) return;
            windowResolutionIterator = 2;
            optionManager->width = 1920;
            optionManager->height = 1080;
            PostProcessingManager::GetInstance()->WindowResize();
            glfwSetWindowMonitor(GloomEngine::GetInstance()->window, glfwGetPrimaryMonitor(), mode->width / 2 - optionManager->width / 2, mode->height / 2 - optionManager->height / 2, mode->width, mode->height, mode->refreshRate);
            windowFullScreenIterator++;
            optionManager->fullScreen = true;
            windowResolutionValue->ChangeText(windowResolutionValues[windowResolutionIterator]);
            windowResolutionButtons[0]->isActive = true;
            windowResolutionButtons[1]->isActive = false;
        }
        windowFullScreenButtons[0]->isActive = true;
        windowFullScreenButtons[1]->isActive = true;
        if (windowFullScreenIterator == 0) windowFullScreenButtons[0]->isActive = false;
        if (windowFullScreenIterator == 1) windowFullScreenButtons[1]->isActive = false;
        windowFullScreenValue->ChangeText(windowFullScreenValues[windowFullScreenIterator]);
        sound->ForcePlaySound();
    }

	else if (activeButtonName == mainMenu + "ShadowResolutionButton") {
        auto shadowManager = ShadowManager::GetInstance();
        if (x == -1.0f) {
            if (shadowManager->shadowResolution <= 1024) return;
            shadowManager->shadowResolution /= 2;
            shadowResolutionIterator--;
            optionManager->shadowResolution /= 2;
        } else {
            if (shadowManager->shadowResolution >= 4096) return;
            shadowManager->shadowResolution *= 2;
            shadowResolutionIterator++;
            optionManager->shadowResolution *= 2;
        }
        shadowResolutionButtons[0]->isActive = true;
        shadowResolutionButtons[1]->isActive = true;
        if (shadowResolutionIterator == 0) shadowResolutionButtons[0]->isActive = false;
        if (shadowResolutionIterator == 2) shadowResolutionButtons[1]->isActive = false;
        ChangeShadowResolution();
        shadowResolutionValue->ChangeText(shadowResolutionValues[shadowResolutionIterator]);
        sound->ForcePlaySound();
    }
}

void OptionsMenu::ChangeShadowResolution() {
    auto shadowManager = ShadowManager::GetInstance();
    glGenFramebuffers(1, &shadowManager->depthMapFBO);
    glGenTextures(1, &shadowManager->depthMap);
    glBindTexture(GL_TEXTURE_2D, shadowManager->depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowManager->shadowResolution, shadowManager->shadowResolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    glBindFramebuffer(GL_FRAMEBUFFER, shadowManager->depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowManager->depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    RendererManager::GetInstance()->shader->Activate();
    RendererManager::GetInstance()->shader->SetInt("shadowMap", shadowManager->depthMap);
}

void OptionsMenu::CancelSettings() {
    auto optionManager = OptionsManager::GetInstance();
//     previousMusicVolume
    if (GloomEngine::GetInstance()->FindGameObjectWithName("Player"))
        GloomEngine::GetInstance()->FindGameObjectWithName("Player")->GetComponent<AudioListener>()->SetGain(previousMusicVolume);
    optionManager->musicVolume = previousMusicVolume;
    musicVolumeIterator = (short)(optionManager->musicVolume / 0.125f);
    musicVolumeButtons[0]->isActive = true;
    musicVolumeButtons[1]->isActive = true;
    if (musicVolumeIterator == 0) musicVolumeButtons[0]->isActive = false;
    if (musicVolumeIterator == 8) musicVolumeButtons[1]->isActive = false;
    musicVolumeValue->ChangeText(musicVolumeValues[musicVolumeIterator]);

//     previousWindowResolutionWidth
    const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    optionManager->width = (int)previousWindowResolutionWidth;
    optionManager->height = (int)previousWindowResolutionHeight;
    if (optionManager->width == 1920)   glfwSetWindowPos(GloomEngine::GetInstance()->window, mode->width / 2 - optionManager->width / 2, mode->height / 2 - optionManager->height / 2);
    if (optionManager->width == 1440)   glfwSetWindowPos(GloomEngine::GetInstance()->window, mode->width / 2 - optionManager->width / 2, mode->height / 2 - optionManager->height / 2);
    if (optionManager->width == 960)   glfwSetWindowPos(GloomEngine::GetInstance()->window, mode->width / 2 - optionManager->width / 2, mode->height / 2 - optionManager->height / 2);
    windowResolutionIterator = (short)(optionManager->width / 480 - 2);
    windowResolutionButtons[0]->isActive = true;
    windowResolutionButtons[1]->isActive = true;
    if (windowResolutionIterator == 0) windowResolutionButtons[0]->isActive = false;
    if (windowResolutionIterator == 2) windowResolutionButtons[1]->isActive = false;
    windowResolutionValue->ChangeText(windowResolutionValues[windowResolutionIterator]);

//     previousWindowFullScreen
    if (previousWindowFullScreen == 0.0f) {
        glfwSetWindowMonitor(GloomEngine::GetInstance()->window, NULL,
                             mode->width / 2 - optionManager->width / 2,
                             mode->height / 2 - optionManager->height / 2,
                             optionManager->width,
                             optionManager->height, 0);
        optionManager->fullScreen = false;
    } else {
        optionManager->fullScreen = true;
        glfwSetWindowMonitor(GloomEngine::GetInstance()->window, glfwGetPrimaryMonitor(), 0, 0, mode->width, mode->height, mode->refreshRate);
    }
    windowFullScreenIterator = (short)(optionManager->fullScreen);
    windowFullScreenButtons[0]->isActive = true;
    windowFullScreenButtons[1]->isActive = true;
    if (windowFullScreenIterator == 0) windowFullScreenButtons[0]->isActive = false;
    if (windowFullScreenIterator == 1) windowFullScreenButtons[1]->isActive = false;
    windowFullScreenValue->ChangeText(windowFullScreenValues[windowFullScreenIterator]);
    PostProcessingManager::GetInstance()->WindowResize();

//     previousShadowResolution
    ShadowManager::GetInstance()->shadowResolution = (int)previousShadowResolution;
    optionManager->shadowResolution = (int)previousShadowResolution;
    shadowResolutionIterator = (short)(optionManager->shadowResolution / 2048);
    shadowResolutionButtons[0]->isActive = true;
    shadowResolutionButtons[1]->isActive = true;
    if (shadowResolutionIterator == 0) shadowResolutionButtons[0]->isActive = false;
    if (shadowResolutionIterator == 2) shadowResolutionButtons[1]->isActive = false;
    ChangeShadowResolution();
    shadowResolutionValue->ChangeText(shadowResolutionValues[shadowResolutionIterator]);
}

void OptionsMenu::OnDestroy() {
    musicVolumeValue.reset();
    windowResolutionValue.reset();
    windowFullScreenValue.reset();
    shadowResolutionValue.reset();
    sound.reset();
    musicVolumeButtons.clear();
    windowResolutionButtons.clear();
    windowFullScreenButtons.clear();
    shadowResolutionButtons.clear();
    Menu::OnDestroy();
}