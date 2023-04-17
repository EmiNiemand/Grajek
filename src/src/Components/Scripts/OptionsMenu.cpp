#include "Components/Scripts/OptionsMenu.h"
#include "GloomEngine.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/UI/Button.h"
#include "Components/Scripts/PauseMenu.h"
#include "EngineManagers/PostProcessingManager.h"
#include "Components/Audio/AudioListener.h"

OptionsMenu::OptionsMenu(const std::shared_ptr<GameObject> &parent, int id) : Component(parent, id) {}

OptionsMenu::~OptionsMenu() {}

void OptionsMenu::Start() {
    activeButton = GloomEngine::GetInstance()->FindGameObjectWithName("BackToPauseMenu")->GetComponent<Button>();
    Component::Start();
}

void OptionsMenu::ShowMenu() {
    parent->EnableSelfAndChildren();
    activeButton = GloomEngine::GetInstance()->FindGameObjectWithName("BackToPauseMenu")->GetComponent<Button>();
    activeButton->isActive = true;
}

void OptionsMenu::HideMenu() {
    parent->DisableSelfAndChildren();
    activeButton->isActive = false;
}

void OptionsMenu::ChangeActiveButton(glm::vec2 moveVector) {
    if (moveVector.y == 1.0f) {
        activeButton->isActive = false;
        activeButton = activeButton->previousButton;
        activeButton->isActive = true;
    }
    if (moveVector.y == -1.0f) {
        activeButton->isActive = false;
        activeButton = activeButton->nextButton;
        activeButton->isActive = true;
    }
}

void OptionsMenu::OnClick() {
    if(GloomEngine::GetInstance()->FindGameObjectWithName("BackToPauseMenu")->GetComponent<Button>()->isActive) {
        HideMenu();
        GloomEngine::GetInstance()->FindGameObjectWithName("Pause")->GetComponent<PauseMenu>()->ShowMenu();
    } else if (GloomEngine::GetInstance()->FindGameObjectWithName("DecreaseVolume")->GetComponent<Button>()->isActive) {
        float gain = GloomEngine::GetInstance()->FindGameObjectWithName("Player")->GetComponent<AudioListener>()->GetGain();
        if (gain <= 0.0f) return;
        GloomEngine::GetInstance()->FindGameObjectWithName("Player")->GetComponent<AudioListener>()->SetGain(gain - 0.1f);
        GloomEngine::GetInstance()->FindGameObjectWithName("MusicVolumeValue")->GetComponent<Text>()->text = std::to_string((int)std::ceil((gain - 0.1f) * 10));
    } else if (GloomEngine::GetInstance()->FindGameObjectWithName("IncreaseVolume")->GetComponent<Button>()->isActive) {
        float gain = GloomEngine::GetInstance()->FindGameObjectWithName("Player")->GetComponent<AudioListener>()->GetGain();
        if (gain >= 1.0f) return;
        GloomEngine::GetInstance()->FindGameObjectWithName("Player")->GetComponent<AudioListener>()->SetGain(gain + 0.1f);
        GloomEngine::GetInstance()->FindGameObjectWithName("MusicVolumeValue")->GetComponent<Text>()->text = std::to_string((int)std::ceil((gain + 0.1f) * 10));
    } else if (GloomEngine::GetInstance()->FindGameObjectWithName("DecreaseResolution")->GetComponent<Button>()->isActive) {
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
        }
        GloomEngine::GetInstance()->FindGameObjectWithName("WindowResolutionValue")->GetComponent<Text>()->text = std::to_string(GloomEngine::GetInstance()->width) + " x " + std::to_string(GloomEngine::GetInstance()->height);
    } else if (GloomEngine::GetInstance()->FindGameObjectWithName("IncreaseResolution")->GetComponent<Button>()->isActive) {
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
        }
        GloomEngine::GetInstance()->FindGameObjectWithName("WindowResolutionValue")->GetComponent<Text>()->text = std::to_string(GloomEngine::GetInstance()->width) + " x " + std::to_string(GloomEngine::GetInstance()->height);
    }
}