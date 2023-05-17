//
// Created by Mateusz on 04.05.2023.
//

#ifndef GLOOMENGINE_STATICOBJECTDATA_H
#define GLOOMENGINE_STATICOBJECTDATA_H

#include "glm/matrix.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <string>
#include <vector>

class StaticObjectData {
public:
    int positionInList;
    std::string name;
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

public:
StaticObjectData();
};


#endif //GLOOMENGINE_STATICOBJECTDATA_H
