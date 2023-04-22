#include "EngineManagers/UIManager.h"
#include "Components/UI/UIComponent.h"
#include "LowLevelClasses/Shader.h"
#include "stb_image.h"

UIManager::UIManager() {
    shader = std::make_shared<Shader>("UI.vert", "UI.frag");
}

UIManager::~UIManager() = default;

UIManager* UIManager::GetInstance() {
    if (uiManager == nullptr) {
        uiManager = new UIManager();
    }
    return uiManager;
}

void UIManager::Free() const {
    shader->Delete();
}

void UIManager::DrawUI() {
    for (const auto& component : drawBuffer) {
        component->Draw();
    }

    drawBuffer.clear();
}

void UIManager::AddToDrawBuffer(const std::shared_ptr<UIComponent> &component) {
    drawBuffer.emplace_back(component);
}
