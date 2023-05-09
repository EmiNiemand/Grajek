//
// Created by Mateusz on 09.05.2023.
//

#ifndef GLOOMENGINE_SAVEABLESTATICOBJECT_H
#define GLOOMENGINE_SAVEABLESTATICOBJECT_H

#include <memory>
#include "LowLevelClasses/StaticObjData.h"


class SaveableStaticObject {
public:
    //Create shared pointer to object holding data of this object in format suitable for conversion into json
    virtual std::shared_ptr<StaticObjData> SaveStatic() = 0;
    //Update your values using data from passed pointer to instance of StaticObjData
    virtual void LoadStatic(std::shared_ptr<StaticObjData> &data) = 0;
};


#endif //GLOOMENGINE_SAVEABLESTATICOBJECT_H
