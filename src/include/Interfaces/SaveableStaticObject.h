//
// Created by Mateusz on 09.05.2023.
//

#ifndef GLOOMENGINE_SAVEABLESTATICOBJECT_H
#define GLOOMENGINE_SAVEABLESTATICOBJECT_H

#include <memory>
#include "LowLevelClasses/StaticObjectData.h"
#include <glm/gtc/type_ptr.hpp>


class SaveableStaticObject {
public:
    //Create shared pointer to object holding data of this object in format suitable for conversion into json
    virtual std::shared_ptr<StaticObjectData> SaveStatic() = 0;
    //Prefabs are will NOT load in data themselves to modify their state
    //virtual void LoadStatic() = 0;
    //They will instead be modified during creation by SceneManager
};


#endif //GLOOMENGINE_SAVEABLESTATICOBJECT_H
