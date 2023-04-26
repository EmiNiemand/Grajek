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
}

void OptionsMenu::OnClick() {
    if(activeButton->GetParent()->GetName() == "BackToPauseMenu") {
        GloomEngine::GetInstance()->FindGameObjectWithName("Player")->GetComponent<PlayerManager>()->ToggleOptionsMenu();
        HideMenu();
        GloomEngine::GetInstance()->FindGameObjectWithName("Pause")->GetComponent<PauseMenu>()->ShowMenu();
    } else if (activeButton->GetParent()->GetName() == "DecreaseVolume") {
        float gain = GloomEngine::GetInstance()->FindGameObjectWithName("Player")->GetComponent<AudioListener>()->GetGain();
        if (gain <= 0.0f) return;
        if (gain <= 0.11f) gain = 0.1f;
        GloomEngine::GetInstance()->FindGameObjectWithName("Player")->GetComponent<AudioListener>()->SetGain(gain - 0.1f);
        GloomEngine::GetInstance()->FindGameObjectWithName("MusicVolumeValue")->GetComponent<Text>()->text = std::to_string((int)std::ceil((gain - 0.1f) * 10));
    } else if (activeButton->GetParent()->GetName() == "IncreaseVolume") {
        float gain = GloomEngine::GetInstance()->FindGameObjectWithName("Player")->GetComponent<AudioListener>()->GetGain();
        if (gain >= 1.0f) return;
        if (gain >= 0.89f) gain = 0.9f;
        GloomEngine::GetInstance()->FindGameObjectWithName("Player")->GetComponent<AudioListener>()->SetGain(gain + 0.1f);
        GloomEngine::GetInstance()->FindGameObjectWithName("MusicVolumeValue")->GetComponent<Text>()->text = std::to_string((int)std::ceil((gain + 0.1f) * 10));
    } else if (activeButton->GetParent()->GetName() == "DecreaseWindowResolution") {
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
    } else if (activeButton->GetParent()->GetName() == "IncreaseWindowResolution") {
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
    } else if (activeButton->GetParent()->GetName() == "DecreaseShadowResolution" || activeButton->GetParent()->GetName() == "IncreaseShadowResolution") {
        auto shadowManager = ShadowManager::GetInstance();
        if (activeButton->GetParent()->GetName() == "DecreaseShadowResolution") {
            if (shadowManager->shadowResolution <= 1024) return;
            shadowManager->shadowResolution /= 2;
        } else {
            if (shadowManager->shadowResolution >= 4096) return;
            shadowManager->shadowResolution *= 2;
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

        if (shadowManager->shadowResolution == 1024) GloomEngine::GetInstance()->FindGameObjectWithName("ShadowResolutionValue")->GetComponent<Text>()->text = "Low";
        else if (shadowManager->shadowResolution == 2048) GloomEngine::GetInstance()->FindGameObjectWithName("ShadowResolutionValue")->GetComponent<Text>()->text = "Medium";
        else if (shadowManager->shadowResolution == 4096) GloomEngine::GetInstance()->FindGameObjectWithName("ShadowResolutionValue")->GetComponent<Text>()->text = "High";
    }
}