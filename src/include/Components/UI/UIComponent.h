//
// Created by szymo on 05/04/2023.
//

#ifndef GLOOMENGINE_UICOMPONENT_H
#define GLOOMENGINE_UICOMPONENT_H

#include "Components/Component.h"
#include "glm/vec3.hpp"

class UIComponent : public Component{
public:
    UIComponent(const std::shared_ptr<GameObject>& parent, int id);
    ~UIComponent() override;

    void Update() override;
    virtual void Draw() = 0;

    inline virtual void SetPosition(float x2, float y2){};
    inline virtual void SetRotation(float angle){};
    inline virtual void SetScale(float scale){};
    inline virtual void SetColor(glm::vec3 newColor){};
    inline virtual void SetAlpha(float newAlpha){};
    inline virtual glm::vec3 GetColor(){ return {}; };
    inline virtual float GetAlpha(){ return 1.0f; };

private:
    void AddToDraw();
};


#endif //GLOOMENGINE_UICOMPONENT_H
