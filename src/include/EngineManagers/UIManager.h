#ifndef GLOOMENGINE_UIMANAGER_H
#define GLOOMENGINE_UIMANAGER_H

#include <memory>

class Shader;

class UIManager {
private:
    inline static UIManager* uiManager;

public:
    std::shared_ptr<Shader> shader;

public:
    UIManager(UIManager &other) = delete;
    void operator=(const UIManager&) = delete;
    virtual ~UIManager();

    static UIManager* GetInstance();

    void Free() const;

private:
    explicit UIManager();
};

#endif //GLOOMENGINE_UIMANAGER_H
