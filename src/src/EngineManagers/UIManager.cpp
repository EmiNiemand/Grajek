#include "EngineManagers/UIManager.h"
#include "Components/UI/UIComponent.h"
#include "LowLevelClasses/Shader.h"
#include "stb_image.h"

UIManager::UIManager() {
    shader = std::make_shared<Shader>("UI.vert", "UI.frag");
}

UIManager::~UIManager() {
    delete uiManager;
}

UIManager* UIManager::GetInstance() {
    if (uiManager == nullptr) {
        uiManager = new UIManager();
    }
    return uiManager;
}

void UIManager::Free() const {
    shader->Delete();
}

void UIManager::Draw() {
    DrawUI();
    ClearBuffer();
}

void UIManager::DrawUI() {
    for (int i = 0; i < bufferIterator; ++i) {
        drawBuffer[i]->Draw();
    }
}

void UIManager::AddToDrawBuffer(const std::shared_ptr<UIComponent> &component) {
    drawBuffer[bufferIterator] = component;
    ++bufferIterator;
}

void UIManager::ClearBuffer() {
    for (int i = 0; i < bufferIterator; ++i) {
        drawBuffer[i] = nullptr;
    }
    bufferIterator = 0;
}

