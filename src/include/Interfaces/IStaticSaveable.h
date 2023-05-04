//
// Created by Mateusz on 04.05.2023.
//

#ifndef GLOOMENGINE_ISTATICSAVEABLE_H
#define GLOOMENGINE_ISTATICSAVEABLE_H

#include <memory>

class StaticObjData;

class IStaticSaveable {
public:
    virtual void SaveStatic(std::shared_ptr<StaticObjData> data);
    virtual void LoadStatic(std::shared_ptr<StaticObjData> &data);
};


#endif //GLOOMENGINE_ISTATICSAVEABLE_H
