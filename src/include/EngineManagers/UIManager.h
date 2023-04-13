#ifndef GLOOMENGINE_UIMANAGER_H
#define GLOOMENGINE_UIMANAGER_H

#include <memory>
#include <vector>

class Shader;
class UIComponent;

class UIManager {
private:
    inline static UIManager* uiManager;
    std::vector<std::shared_ptr<UIComponent>> drawBuffer;

public:
    std::shared_ptr<Shader> shader;

public:
    UIManager(UIManager &other) = delete;
    void operator=(const UIManager&) = delete;
    virtual ~UIManager();

    static UIManager* GetInstance();

    void Free() const;

    void DrawUI();
    void AddToDrawBuffer(const std::shared_ptr<UIComponent>& component);

private:
    explicit UIManager();
};

#endif //GLOOMENGINE_UIMANAGER_H
