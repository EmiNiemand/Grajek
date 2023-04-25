#include "Components/Scripts/OptionsMenu.h"
#include "GloomEngine.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/UI/Button.h"
#include "Components/Scripts/PauseMenu.h"
#include "EngineManagers/PostProcessingManager.h"
#include "Components/Audio/AudioListener.h"
#include "Components/Scripts/PlayerManager.h"
#include "EngineManagers/ShadowManager.h"
#include "EngineManagers/RendererManager.h"

OptionsMenu::OptionsMenu(const std::shared_ptr<GameObject> &parent, int id) : Menu(parent, id) {}

OptionsMenu::~OptionsMenu() {}

void OptionsMenu::ShowMenu() {
    parent->EnableSelfAndChildren();
    activeButton = GloomEngine::GetInstance()->FindGameObjectWithName("BackToPauseMenu")->GetComponent<Button>();
    activeButton->isActive = true;
    GloomEngine::GetInstance()->FindGameObjectWithName("PreviousValue")->GetComponent<Button>()->ChangeText("");
    GloomEngine::GetInstance()->FindGameObjectWithName("CurrentValue")->GetComponent<Button>()->ChangeText("");
    GloomEngine::GetInstance()->FindGameObjectWithName("NextValue")->GetComponent<Button>()->ChangeText("");
}

void OptionsMenu::ChangeActiveButton(glm::vec2 moveVector) {
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
        GloomEngine::GetInstance()->FindGameObjectWithName("PreviousValue")->GetComponent<Button>()->ChangeText(musicVolumeValues[musicVolumeIterator + 1]);
        GloomEngine::GetInstance()->FindGameObjectWithName("CurrentValue")->GetComponent<Button>()->ChangeText(musicVolumeValues[musicVolumeIterator]);
        GloomEngine::GetInstance()->FindGameObjectWithName("NextValue")->GetComponent<Button>()->ChangeText(musicVolumeValues[musicVolumeIterator - 1]);
    } else if (activeButton->GetParent()->GetName() == "WindowResolution") {
        GloomEngine::GetInstance()->FindGameObjectWithName("PreviousValue")->GetComponent<Button>()->ChangeText(windowResolutionValues[windowResolutionIterator + 1]);
        GloomEngine::GetInstance()->FindGameObjectWithName("CurrentValue")->GetComponent<Button>()->ChangeText(windowResolutionValues[windowResolutionIterator]);
        GloomEngine::GetInstance()->FindGameObjectWithName("NextValue")->GetComponent<Button>()->ChangeText(windowResolutionValues[windowResolutionIterator - 1]);
    } else if (activeButton->GetParent()->GetName() == "WindowFullScreen") {
        GloomEngine::GetInstance()->FindGameObjectWithName("PreviousValue")->GetComponent<Button>()->ChangeText(windowFullScreenValues[windowFullScreenIterator + 1]);
        GloomEngine::GetInstance()->FindGameObjectWithName("CurrentValue")->GetComponent<Button>()->ChangeText(windowFullScreenValues[windowFullScreenIterator]);
        GloomEngine::GetInstance()->FindGameObjectWithName("NextValue")->GetComponent<Button>()->ChangeText(windowFullScreenValues[windowFullScreenIterator - 1]);
    } else if (activeButton->GetParent()->GetName() == "ShadowResolution") {
        GloomEngine::GetInstance()->FindGameObjectWithName("PreviousValue")->GetComponent<Button>()->ChangeText(shadowResolutionValues[shadowResolutionIterator + 1]);
        GloomEngine::GetInstance()->FindGameObjectWithName("CurrentValue")->GetComponent<Button>()->ChangeText(shadowResolutionValues[shadowResolutionIterator]);
        GloomEngine::GetInstance()->FindGameObjectWithName("NextValue")->GetComponent<Button>()->ChangeText(shadowResolutionValues[shadowResolutionIterator - 1]);
    }
}

void OptionsMenu::ChangeValue(float y) {
    if (activeButton->GetParent()->GetName() == "MusicVolume") {
        if (y == -1.0f) {
            float gain = GloomEngine::GetInstance()->FindGameObjectWithName("Player")->GetComponent<AudioListener>()->GetGain();
            if (gain <= 0.0f) return;
            GloomEngine::GetInstance()->FindGameObjectWithName("Player")->GetComponent<AudioListener>()->SetGain(gain - 0.125f);
            musicVolumeIterator--;
        } else {
            float gain = GloomEngine::GetInstance()->FindGameObjectWithName("Player")->GetComponent<AudioListener>()->GetGain();
            if (gain >= 1.0f) return;
            GloomEngine::GetInstance()->FindGameObjectWithName("Player")->GetComponent<AudioListener>()->SetGain(gain + 0.125f);
            musicVolumeIterator++;
        }
        GloomEngine::GetInstance()->FindGameObjectWithName("PreviousValue")->GetComponent<Button>()->ChangeText(musicVolumeValues[musicVolumeIterator + 1]);
        GloomEngine::GetInstance()->FindGameObjectWithName("CurrentValue")->GetComponent<Button>()->ChangeText(musicVolumeValues[musicVolumeIterator]);
        GloomEngine::GetInstance()->FindGameObjectWithName("NextValue")->GetComponent<Button>()->ChangeText(musicVolumeValues[musicVolumeIterator - 1]);
        auto button = GloomEngine::GetInstance()->FindGameObjectWithName("MusicVolume")->GetComponent<Button>();
        button->ChangePosition(button->x, musicVolumeButtonY[musicVolumeIterator]);
    } else if (activeButton->GetParent()->GetName() == "WindowResolution") {
        if (y == -1.0f) {
            if (GloomEngine::GetInstance()->width == 1920) {
                glfwSetWindowPos(GloomEngine::GetInstance()->window, 240, 135);
                GloomEngine::GetInstance()->width = 1440;
                GloomEngine::GetInstance()->height = 810;
                PostProcessingManager::GetInstance()->WindowResize();
            } else if (GloomEngine::GetInstance()->width == 1440) {
                glfwSetWindowPos(GloomEngine::GetInstance()->window, 480, 270);
                GloomEngine::GetInstance()->width = 960;
                GloomEngine::GetInstance()->height = 540;
                PostProcessingManager::GetInstance()->WindowResize();
            } else return;
            windowResolutionIterator--;
        } else {
            if (GloomEngine::GetInstance()->width == 960) {
                glfwSetWindowPos(GloomEngine::GetInstance()->window, 240, 135);
                GloomEngine::GetInstance()->width = 1440;
                GloomEngine::GetInstance()->height = 810;
                PostProcessingManager::GetInstance()->WindowResize();
            } else if (GloomEngine::GetInstance()->width == 1440) {
                glfwSetWindowPos(GloomEngine::GetInstance()->window, 0, 0);
                GloomEngine::GetInstance()->width = 1920;
                GloomEngine::GetInstance()->height = 1080;
                PostProcessingManager::GetInstance()->WindowResize();
            } else return;
            windowResolutionIterator++;
        }
        if (windowFullScreenIterator == 2) {
            auto gloomEngine = GloomEngine::GetInstance();
            glfwSetWindowMonitor(gloomEngine->window, NULL, (1920 - gloomEngine->width) / 2, (1080 - gloomEngine->height) / 2, gloomEngine->width, gloomEngine->height, 0);
            windowFullScreenIterator--;
            auto button = GloomEngine::GetInstance()->FindGameObjectWithName("WindowFullScreen")->GetComponent<Button>();
            button->ChangePosition(button->x, windowFullScreenButtonY[windowFullScreenIterator]);
        }
        GloomEngine::GetInstance()->FindGameObjectWithName("PreviousValue")->GetComponent<Button>()->ChangeText(windowResolutionValues[windowResolutionIterator + 1]);
        GloomEngine::GetInstance()->FindGameObjectWithName("CurrentValue")->GetComponent<Button>()->ChangeText(windowResolutionValues[windowResolutionIterator]);
        GloomEngine::GetInstance()->FindGameObjectWithName("NextValue")->GetComponent<Button>()->ChangeText(windowResolutionValues[windowResolutionIterator - 1]);
        auto button = GloomEngine::GetInstance()->FindGameObjectWithName("WindowResolution")->GetComponent<Button>();
        button->ChangePosition(button->x, windowResolutionButtonY[windowResolutionIterator]);
    } else if (activeButton->GetParent()->GetName() == "WindowFullScreen") {
        if (y == -1.0f) {
            if (windowFullScreenIterator == 1) return;
            auto gloomEngine = GloomEngine::GetInstance();
            glfwSetWindowMonitor(gloomEngine->window, NULL, (1920 - gloomEngine->width) / 2, (1080 - gloomEngine->height) / 2, gloomEngine->width, gloomEngine->height, 0);
            windowFullScreenIterator--;
        } else {
            if (windowFullScreenIterator == 2) return;
            GLFWmonitor* monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode* mode = glfwGetVideoMode(monitor);
            windowResolutionIterator = 3;
            auto button = GloomEngine::GetInstance()->FindGameObjectWithName("WindowResolution")->GetComponent<Button>();
            button->ChangePosition(button->x, windowResolutionButtonY[windowResolutionIterator]);
            GloomEngine::GetInstance()->width = mode->width;
            GloomEngine::GetInstance()->height = mode->height;
            PostProcessingManager::GetInstance()->WindowResize();
            glfwSetWindowMonitor(GloomEngine::GetInstance()->window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
            windowFullScreenIterator++;
        }
        GloomEngine::GetInstance()->FindGameObjectWithName("PreviousValue")->GetComponent<Button>()->ChangeText(windowFullScreenValues[windowFullScreenIterator + 1]);
        GloomEngine::GetInstance()->FindGameObjectWithName("CurrentValue")->GetComponent<Button>()->ChangeText(windowFullScreenValues[windowFullScreenIterator]);
        GloomEngine::GetInstance()->FindGameObjectWithName("NextValue")->GetComponent<Button>()->ChangeText(windowFullScreenValues[windowFullScreenIterator - 1]);
        auto button = GloomEngine::GetInstance()->FindGameObjectWithName("WindowFullScreen")->GetComponent<Button>();
        button->ChangePosition(button->x, windowFullScreenButtonY[windowFullScreenIterator]);
    } else if (activeButton->GetParent()->GetName() == "ShadowResolution") {
        auto shadowManager = ShadowManager::GetInstance();
        if (y == -1.0f) {
            if (shadowManager->shadowResolution <= 1024) return;
            shadowManager->shadowResolution /= 2;
            shadowResolutionIterator--;
        } else {
            if (shadowManager->shadowResolution >= 4096) return;
            shadowManager->shadowResolution *= 2;
            shadowResolutionIterator++;
        }
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
        GloomEngine::GetInstance()->FindGameObjectWithName("PreviousValue")->GetComponent<Button>()->ChangeText(shadowResolutionValues[shadowResolutionIterator + 1]);
        GloomEngine::GetInstance()->FindGameObjectWithName("CurrentValue")->GetComponent<Button>()->ChangeText(shadowResolutionValues[shadowResolutionIterator]);
        GloomEngine::GetInstance()->FindGameObjectWithName("NextValue")->GetComponent<Button>()->ChangeText(shadowResolutionValues[shadowResolutionIterator - 1]);
        auto button = GloomEngine::GetInstance()->FindGameObjectWithName("ShadowResolution")->GetComponent<Button>();
        button->ChangePosition(button->x, shadowResolutionButtonY[shadowResolutionIterator]);
    }
}

void OptionsMenu::OnClick() {
    if(activeButton->GetParent()->GetName() == "BackToPauseMenu") {
        GloomEngine::GetInstance()->FindGameObjectWithName("Player")->GetComponent<PlayerManager>()->ToggleOptionsMenu();
        HideMenu();
        GloomEngine::GetInstance()->FindGameObjectWithName("Pause")->GetComponent<PauseMenu>()->ShowMenu();
    }
}