//
// Created by Mateusz on 04.05.2023.
//

#ifndef GLOOMENGINE_STATICOBJDATA_H
#define GLOOMENGINE_STATICOBJDATA_H

#include "glm/matrix.hpp"
#include "glm/gtc/matrix_transform.hpp"

class StaticObjData {
public:
    glm::vec3 position;
    glm::vec3 rot;
    glm::vec3 scale;

public:
StaticObjData();
};


#endif //GLOOMENGINE_STATICOBJDATA_H
