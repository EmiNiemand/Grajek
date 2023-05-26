#ifndef GLOOMENGINE_UIMANAGER_H
#define GLOOMENGINE_UIMANAGER_H

#include <memory>
#include <vector>

class Shader;
class UIComponent;

class UIManager {
private:
    unsigned int bufferIterator = 0;

    inline static UIManager* uiManager;
    std::shared_ptr<UIComponent> drawBuffer[5000];

public:
    std::shared_ptr<Shader> shader;

public:
    UIManager(UIManager &other) = delete;
    void operator=(const UIManager&) = delete;
    virtual ~UIManager();

    static UIManager* GetInstance();

    void Free() const;

    void Draw();
    void DrawUI();
    void AddToDrawBuffer(const std::shared_ptr<UIComponent>& component);

private:
    explicit UIManager();

    void ClearBuffer();
};

#endif //GLOOMENGINE_UIMANAGER_H
