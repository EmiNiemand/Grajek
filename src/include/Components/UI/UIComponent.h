//
// Created by szymo on 05/04/2023.
//

#ifndef GLOOMENGINE_UICOMPONENT_H
#define GLOOMENGINE_UICOMPONENT_H

#include "Components/Component.h"

class UIComponent : public Component{
public:
    UIComponent(const std::shared_ptr<GameObject>& parent, int id);
    ~UIComponent() override;

    void Update() override;
    virtual void Draw() = 0;

private:
    void AddToDraw();
};


#endif //GLOOMENGINE_UICOMPONENT_H
