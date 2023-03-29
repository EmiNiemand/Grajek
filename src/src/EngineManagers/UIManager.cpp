#include "EngineManagers/UIManager.h"
#include "LowLevelClasses/Shader.h"
#include "stb_image.h"

UIManager* UIManager::uiManager = nullptr;

UIManager::UIManager() {
    shader = std::make_shared<Shader>("UI.vert", "UI.frag");
}

UIManager::~UIManager() {}

UIManager* UIManager::GetInstance() {
    if (uiManager == nullptr) {
        uiManager = new UIManager();
    }
    return uiManager;
}

void UIManager::Free() {
    shader->Delete();
}