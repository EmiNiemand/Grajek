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

OptionsMenu::OptionsMenu(const std::shared_ptr<GameObject> &parent, int id) : Menu(parent, id) {
    musicVolumeIterator = (short)(OptionsManager::GetInstance()->musicVolume / 0.125f + 1);
    windowResolutionIterator = (short)(OptionsManager::GetInstance()->width / 480 - 1);
    windowFullScreenIterator = (short)(OptionsManager::GetInstance()->fullScreen + 1);
    shadowResolutionIterator = (short)(OptionsManager::GetInstance()->shadowResolution / 2048 + 1);
    GloomEngine::GetInstance()->FindGameObjectWithName("Player")->GetComponent<AudioListener>()->SetGain(OptionsManager::GetInstance()->musicVolume);
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

OptionsMenu::~OptionsMenu() = default;

void OptionsMenu::Start() {
    auto button = GloomEngine::GetInstance()->FindGameObjectWithName("MusicVolume")->GetComponent<Button>();
    button->ChangePosition(button->x, musicVolumeButtonY[musicVolumeIterator]);
    button = GloomEngine::GetInstance()->FindGameObjectWithName("WindowResolution")->GetComponent<Button>();
    button->ChangePosition(button->x, windowResolutionButtonY[windowResolutionIterator]);
    button = GloomEngine::GetInstance()->FindGameObjectWithName("WindowFullScreen")->GetComponent<Button>();
    button->ChangePosition(button->x, windowFullScreenButtonY[windowFullScreenIterator]);
    button = GloomEngine::GetInstance()->FindGameObjectWithName("ShadowResolution")->GetComponent<Button>();
    button->ChangePosition(button->x, shadowResolutionButtonY[shadowResolutionIterator]);

    GameObject::Instantiate("Background", parent)->AddComponent<Image>()->LoadTexture(0, 0, "UI/backgroundOpacity90.png");
    scroll = parent->AddComponent<AudioSource>();
    scroll->LoadAudioData("res/sounds/direct/options_scroll.wav", AudioType::Direct);

    Component::Start();
}

void OptionsMenu::ShowMenu() {
    parent->EnableSelfAndChildren();
    activeButton = GloomEngine::GetInstance()->FindGameObjectWithName("BackToPauseMenu")->GetComponent<Button>();
    activeButton->isActive = true;
    GloomEngine::GetInstance()->FindGameObjectWithName("PreviousValue")->GetComponent<Button>()->ChangeText("");
    GloomEngine::GetInstance()->FindGameObjectWithName("CurrentValue")->GetComponent<Button>()->ChangeText("");
    GloomEngine::GetInstance()->FindGameObjectWithName("NextValue")->GetComponent<Button>()->ChangeText("");
}

void OptionsMenu::ChangeActiveButton(glm::vec2 moveVector) {
	auto previousValue = GloomEngine::GetInstance()->FindGameObjectWithName("PreviousValue")->GetComponent<Button>();
	auto currentValue = GloomEngine::GetInstance()->FindGameObjectWithName("CurrentValue")->GetComponent<Button>();
	auto nextValue = GloomEngine::GetInstance()->FindGameObjectWithName("NextValue")->GetComponent<Button>();

    if (moveVector.y != 0.0f) {
        ChangeValue(moveVector.y);
        return;
    }
    if (moveVector.x == 1.0f) {
        activeButton->isActive = false;
        activeButton = activeButton->nextButton;
        activeButton->isActive = true;
    }
    if (moveVector.x == -1.0f) {
        activeButton->isActive = false;
        activeButton = activeButton->previousButton;
        activeButton->isActive = true;
    }
    if (activeButton->GetParent()->GetName() == "MusicVolume") {
        previousValue->ChangeText(musicVolumeValues[OptionsManager::GetInstance()->musicVolume + 0.125f]);
        currentValue->ChangeText(musicVolumeValues[OptionsManager::GetInstance()->musicVolume]);
        nextValue->ChangeText(musicVolumeValues[OptionsManager::GetInstance()->musicVolume - 0.125f]);
    } else if (activeButton->GetParent()->GetName() == "WindowResolution") {
        previousValue->ChangeText(windowResolutionValues[windowResolutionIterator + 1]);
        currentValue->ChangeText(windowResolutionValues[windowResolutionIterator]);
        nextValue->ChangeText(windowResolutionValues[windowResolutionIterator - 1]);
    } else if (activeButton->GetParent()->GetName() == "WindowFullScreen") {
        previousValue->ChangeText(windowFullScreenValues[windowFullScreenIterator + 1]);
        currentValue->ChangeText(windowFullScreenValues[windowFullScreenIterator]);
        nextValue->ChangeText(windowFullScreenValues[windowFullScreenIterator - 1]);
    } else if (activeButton->GetParent()->GetName() == "ShadowResolution") {
        previousValue->ChangeText(shadowResolutionValues[shadowResolutionIterator + 1]);
        currentValue->ChangeText(shadowResolutionValues[shadowResolutionIterator]);
        nextValue->ChangeText(shadowResolutionValues[shadowResolutionIterator - 1]);
    }
}

void OptionsMenu::ChangeValue(float y) {
	std::shared_ptr<Button> button;
	auto previousValue = GloomEngine::GetInstance()->FindGameObjectWithName("PreviousValue")->GetComponent<Button>();
	auto currentValue = GloomEngine::GetInstance()->FindGameObjectWithName("CurrentValue")->GetComponent<Button>();
	auto nextValue = GloomEngine::GetInstance()->FindGameObjectWithName("NextValue")->GetComponent<Button>();

	auto activeButtonName = activeButton->GetParent()->GetName();
    if (activeButtonName == "MusicVolume") {
		auto audioListener = GloomEngine::GetInstance()->FindGameObjectWithName("Player")->GetComponent<AudioListener>();
		float gain = audioListener->GetGain();
        if (y == -1.0f) {
            if (gain <= 0.0f) return;
            audioListener->SetGain(gain - 0.125f);
            OptionsManager::GetInstance()->musicVolume -= 0.125f;
            musicVolumeIterator--;
        } else {
            if (gain >= 1.0f) return;
            audioListener->SetGain(gain + 0.125f);
            OptionsManager::GetInstance()->musicVolume += 0.125f;
            musicVolumeIterator++;
        }
        previousValue->ChangeText(musicVolumeValues[OptionsManager::GetInstance()->musicVolume + 0.125f]);
        currentValue->ChangeText(musicVolumeValues[OptionsManager::GetInstance()->musicVolume]);
        nextValue->ChangeText(musicVolumeValues[OptionsManager::GetInstance()->musicVolume - 0.125f]);
        button = GloomEngine::GetInstance()->FindGameObjectWithName("MusicVolume")->GetComponent<Button>();
        button->ChangePosition(button->x, musicVolumeButtonY[musicVolumeIterator]);
        scroll->ForcePlaySound();
    }
	else if (activeButtonName == "WindowResolution") {
        if (y == -1.0f) {
            if (OptionsManager::GetInstance()->width == 1920) {
                glfwSetWindowPos(GloomEngine::GetInstance()->window, 240, 135);
                OptionsManager::GetInstance()->width = 1440;
                OptionsManager::GetInstance()->height = 810;
                PostProcessingManager::GetInstance()->WindowResize();
            } else if (OptionsManager::GetInstance()->width == 1440) {
                glfwSetWindowPos(GloomEngine::GetInstance()->window, 480, 270);
                OptionsManager::GetInstance()->width = 960;
                OptionsManager::GetInstance()->height = 540;
                PostProcessingManager::GetInstance()->WindowResize();
            } else return;
            windowResolutionIterator--;
        } else {
            if (OptionsManager::GetInstance()->width == 960) {
                glfwSetWindowPos(GloomEngine::GetInstance()->window, 240, 135);
                OptionsManager::GetInstance()->width = 1440;
                OptionsManager::GetInstance()->height = 810;
                PostProcessingManager::GetInstance()->WindowResize();
            } else if (OptionsManager::GetInstance()->width == 1440) {
                glfwSetWindowPos(GloomEngine::GetInstance()->window, 0, 0);
                OptionsManager::GetInstance()->width = 1920;
                OptionsManager::GetInstance()->height = 1080;
                PostProcessingManager::GetInstance()->WindowResize();
            } else return;
            windowResolutionIterator++;
        }
        if (windowFullScreenIterator == 2) {
            auto gloomEngine = GloomEngine::GetInstance();
            glfwSetWindowMonitor(gloomEngine->window, NULL,
								 (1920 - OptionsManager::GetInstance()->width) / 2,
								 (1080 - OptionsManager::GetInstance()->height) / 2,
								 OptionsManager::GetInstance()->width,
								 OptionsManager::GetInstance()->height, 0);
            windowFullScreenIterator--;
            OptionsManager::GetInstance()->fullScreen = false;
            button = GloomEngine::GetInstance()->FindGameObjectWithName("WindowFullScreen")->GetComponent<Button>();
            button->ChangePosition(button->x, windowFullScreenButtonY[windowFullScreenIterator]);
        }
        previousValue->ChangeText(windowResolutionValues[windowResolutionIterator + 1]);
        currentValue->ChangeText(windowResolutionValues[windowResolutionIterator]);
        nextValue->ChangeText(windowResolutionValues[windowResolutionIterator - 1]);
        button = GloomEngine::GetInstance()->FindGameObjectWithName("WindowResolution")->GetComponent<Button>();
		button->ChangePosition(button->x, windowResolutionButtonY[windowResolutionIterator]);
        scroll->ForcePlaySound();
    }

	else if (activeButtonName == "WindowFullScreen") {
        if (y == -1.0f) {
            if (windowFullScreenIterator == 1) return;
            glfwSetWindowMonitor(GloomEngine::GetInstance()->window, NULL,
								 (1920 - OptionsManager::GetInstance()->width) / 2,
								 (1080 - OptionsManager::GetInstance()->height) / 2,
								 OptionsManager::GetInstance()->width,
								 OptionsManager::GetInstance()->height, 0);
            windowFullScreenIterator--;
            OptionsManager::GetInstance()->fullScreen = false;
        } else {
            if (windowFullScreenIterator == 2) return;
            GLFWmonitor* monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode* mode = glfwGetVideoMode(monitor);
            windowResolutionIterator = 3;
            button = GloomEngine::GetInstance()->FindGameObjectWithName("WindowResolution")->GetComponent<Button>();
            button->ChangePosition(button->x, windowResolutionButtonY[windowResolutionIterator]);
            OptionsManager::GetInstance()->width = mode->width;
            OptionsManager::GetInstance()->height = mode->height;
            PostProcessingManager::GetInstance()->WindowResize();
            glfwSetWindowMonitor(GloomEngine::GetInstance()->window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
            windowFullScreenIterator++;
            OptionsManager::GetInstance()->fullScreen = true;
        }
        previousValue->ChangeText(windowFullScreenValues[windowFullScreenIterator + 1]);
        currentValue->ChangeText(windowFullScreenValues[windowFullScreenIterator]);
        nextValue->ChangeText(windowFullScreenValues[windowFullScreenIterator - 1]);
        button = GloomEngine::GetInstance()->FindGameObjectWithName("WindowFullScreen")->GetComponent<Button>();
		button->ChangePosition(button->x, windowFullScreenButtonY[windowFullScreenIterator]);
        scroll->ForcePlaySound();
    }

	else if (activeButtonName == "ShadowResolution") {
        auto shadowManager = ShadowManager::GetInstance();
        if (y == -1.0f) {
            if (shadowManager->shadowResolution <= 1024) return;
            shadowManager->shadowResolution /= 2;
            shadowResolutionIterator--;
            OptionsManager::GetInstance()->shadowResolution /= 2;
        } else {
            if (shadowManager->shadowResolution >= 4096) return;
            shadowManager->shadowResolution *= 2;
            shadowResolutionIterator++;
            OptionsManager::GetInstance()->shadowResolution *= 2;
        }
        ChangeShadowResolution();
        previousValue->ChangeText(shadowResolutionValues[shadowResolutionIterator + 1]);
        currentValue->ChangeText(shadowResolutionValues[shadowResolutionIterator]);
        nextValue->ChangeText(shadowResolutionValues[shadowResolutionIterator - 1]);
        button = GloomEngine::GetInstance()->FindGameObjectWithName("ShadowResolution")->GetComponent<Button>();
		button->ChangePosition(button->x, shadowResolutionButtonY[shadowResolutionIterator]);
        scroll->ForcePlaySound();
    }
}

void OptionsMenu::OnClick() {
    if(activeButton->GetParent()->GetName() == "BackToPauseMenu") {
        OptionsManager::GetInstance()->Save();
        GloomEngine::GetInstance()->FindGameObjectWithName("Player")->GetComponent<PlayerManager>()->ToggleOptionsMenu();
        HideMenu();
        GloomEngine::GetInstance()->FindGameObjectWithName("Pause")->GetComponent<PauseMenu>()->ShowMenu();
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

void OptionsMenu::OnDestroy() {
    scroll.reset();
    Menu::OnDestroy();
}