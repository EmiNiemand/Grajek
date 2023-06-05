#include "Components/Scripts/Menus/MainMenuOptionsMenu.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/UI/Button.h"
#include "EngineManagers/OptionsManager.h"
#include "Components/Scripts/Menus/MainMenuManager.h"
#include "Components/Scripts/Menus/MainMenu.h"
#include "GloomEngine.h"
#include "EngineManagers/PostProcessingManager.h"
#include "EngineManagers/ShadowManager.h"
#include "EngineManagers/RendererManager.h"
#include "Components/Audio/AudioSource.h"

MainMenuOptionsMenu::MainMenuOptionsMenu(const std::shared_ptr<GameObject> &parent, int id) : Menu(parent, id) {
    musicVolumeIterator = (short)(OptionsManager::GetInstance()->musicVolume / 0.125f);
    windowResolutionIterator = (short)(OptionsManager::GetInstance()->width / 480 - 2);
    windowFullScreenIterator = (short)(OptionsManager::GetInstance()->fullScreen);
    shadowResolutionIterator = (short)(OptionsManager::GetInstance()->shadowResolution / 2048);
    if (OptionsManager::GetInstance()->fullScreen) {
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        glfwSetWindowMonitor(GloomEngine::GetInstance()->window, monitor,
                             0, 0, mode->width, mode->height, mode->refreshRate);
    } else {
        glfwSetWindowMonitor(GloomEngine::GetInstance()->window, NULL,
                             (1920 - OptionsManager::GetInstance()->width) / 2,
                             (1080 - OptionsManager::GetInstance()->height) / 2,
                             OptionsManager::GetInstance()->width,
                             OptionsManager::GetInstance()->height, 0);
    }
    ShadowManager::GetInstance()->shadowResolution = OptionsManager::GetInstance()->shadowResolution;
    ChangeShadowResolution();
}

MainMenuOptionsMenu::~MainMenuOptionsMenu() = default;

void MainMenuOptionsMenu::Start() {
    musicVolumeValue = GloomEngine::GetInstance()->FindGameObjectWithName("MusicVolumeValue")->GetComponent<Button>();
    windowResolutionValue = GloomEngine::GetInstance()->FindGameObjectWithName("WindowResolutionValue")->GetComponent<Button>();
    windowFullScreenValue = GloomEngine::GetInstance()->FindGameObjectWithName("WindowFullScreenValue")->GetComponent<Button>();
    shadowResolutionValue = GloomEngine::GetInstance()->FindGameObjectWithName("ShadowResolutionValue")->GetComponent<Button>();
    musicVolumeValue->ChangeText(musicVolumeValues[musicVolumeIterator]);
    windowResolutionValue->ChangeText(windowResolutionValues[windowResolutionIterator]);
    windowFullScreenValue->ChangeText(windowFullScreenValues[windowFullScreenIterator]);
    shadowResolutionValue->ChangeText(shadowResolutionValues[shadowResolutionIterator]);
    musicVolumeButtons.push_back(GloomEngine::GetInstance()->FindGameObjectWithName("MusicVolumeLeft")->GetComponent<Button>());
    musicVolumeButtons.push_back(GloomEngine::GetInstance()->FindGameObjectWithName("MusicVolumeRight")->GetComponent<Button>());
    windowResolutionButtons.push_back(GloomEngine::GetInstance()->FindGameObjectWithName("WindowResolutionLeft")->GetComponent<Button>());
    windowResolutionButtons.push_back(GloomEngine::GetInstance()->FindGameObjectWithName("WindowResolutionRight")->GetComponent<Button>());
    windowFullScreenButtons.push_back(GloomEngine::GetInstance()->FindGameObjectWithName("WindowFullScreenLeft")->GetComponent<Button>());
    windowFullScreenButtons.push_back(GloomEngine::GetInstance()->FindGameObjectWithName("WindowFullScreenRight")->GetComponent<Button>());
    shadowResolutionButtons.push_back(GloomEngine::GetInstance()->FindGameObjectWithName("ShadowResolutionLeft")->GetComponent<Button>());
    shadowResolutionButtons.push_back(GloomEngine::GetInstance()->FindGameObjectWithName("ShadowResolutionRight")->GetComponent<Button>());

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

void MainMenuOptionsMenu::ShowMenu() {
    parent->EnableSelfAndChildren();
    activeButton = GloomEngine::GetInstance()->FindGameObjectWithName("MusicVolumeButton")->GetComponent<Button>();
    activeButton->isActive = true;
}

void MainMenuOptionsMenu::ChangeActiveButton(glm::vec2 moveVector) {
    Menu::ChangeActiveButton(moveVector);
    if (moveVector.x != 0.0f)
        ChangeValue(moveVector.x);
}

void MainMenuOptionsMenu::OnClick() {
    if(activeButton->GetParent()->GetName() == "SaveButton") {
        auto mainMenuManager = GloomEngine::GetInstance()->FindGameObjectWithName("MainMenuManager")->GetComponent<MainMenuManager>();
        OptionsManager::GetInstance()->Save();
        HideMenu();
        mainMenuManager->activeMenu = mainMenuManager->mainMenu;
        mainMenuManager->mainMenu->ShowMenu();
    }
    Menu::OnClick();
}

void MainMenuOptionsMenu::ChangeValue(float x) {
    auto activeButtonName = activeButton->GetParent()->GetName();
    auto optionManager = OptionsManager::GetInstance();

    if (activeButtonName == "MusicVolumeButton") {
        if (x == -1.0f) {
            if (optionManager->musicVolume <= 0.0f) return;
            optionManager->musicVolume -= 0.125f;
            musicVolumeIterator--;
        } else {
            if (optionManager->musicVolume >= 1.0f) return;
            optionManager->musicVolume += 0.125f;
            musicVolumeIterator++;
        }
        musicVolumeButtons[0]->isActive = true;
        musicVolumeButtons[1]->isActive = true;
        if (musicVolumeIterator == 0) musicVolumeButtons[0]->isActive = false;
        if (musicVolumeIterator == 8) musicVolumeButtons[1]->isActive = false;
        musicVolumeValue->ChangeText(musicVolumeValues[musicVolumeIterator]);
        sound->ForcePlaySound();
    }
    else if (activeButtonName == "WindowResolutionButton") {
        if (x == -1.0f) {
            if (optionManager->width == 1920) {
                glfwSetWindowPos(GloomEngine::GetInstance()->window, 240, 135);
                optionManager->width = 1440;
                optionManager->height = 810;
            } else if (optionManager->width == 1440) {
                glfwSetWindowPos(GloomEngine::GetInstance()->window, 480, 270);
                optionManager->width = 960;
                optionManager->height = 540;
            } else return;
            windowResolutionIterator--;
        } else {
            if (optionManager->width == 960) {
                glfwSetWindowPos(GloomEngine::GetInstance()->window, 240, 135);
                optionManager->width = 1440;
                optionManager->height = 810;
            } else if (optionManager->width == 1440) {
                glfwSetWindowPos(GloomEngine::GetInstance()->window, 0, 0);
                optionManager->width = 1920;
                optionManager->height = 1080;
            } else return;
            windowResolutionIterator++;
        }
        if (windowFullScreenIterator == 1) {
            auto gloomEngine = GloomEngine::GetInstance();
            glfwSetWindowMonitor(gloomEngine->window, NULL,
                                 (1920 - optionManager->width) / 2,
                                 (1080 - optionManager->height) / 2,
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
    else if (activeButtonName == "WindowFullScreenButton") {
        if (x == -1.0f) {
            if (windowFullScreenIterator == 0) return;
            glfwSetWindowMonitor(GloomEngine::GetInstance()->window, NULL,
                                 (1920 - optionManager->width) / 2,
                                 (1080 - optionManager->height) / 2,
                                 optionManager->width,
                                 optionManager->height, 0);
            windowFullScreenIterator--;
            optionManager->fullScreen = false;
        } else {
            if (windowFullScreenIterator == 1) return;
            GLFWmonitor* monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode* mode = glfwGetVideoMode(monitor);
            windowResolutionIterator = 2;
            optionManager->width = mode->width;
            optionManager->height = mode->height;
            PostProcessingManager::GetInstance()->WindowResize();
            glfwSetWindowMonitor(GloomEngine::GetInstance()->window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
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
    else if (activeButtonName == "ShadowResolutionButton") {
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

void MainMenuOptionsMenu::ChangeShadowResolution() {
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

void MainMenuOptionsMenu::OnDestroy() {
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