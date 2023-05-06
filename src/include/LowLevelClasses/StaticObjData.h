//
// Created by Mateusz on 04.05.2023.
//

#ifndef GLOOMENGINE_STATICOBJDATA_H
#define GLOOMENGINE_STATICOBJDATA_H

#include "glm/matrix.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <string>
#include <vector>

class StaticObjData {
public:
    std::string name;
    int id;
    std::string parentName;

    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    std::vector<std::string> tags;

public:
StaticObjData();
};


#endif //GLOOMENGINE_STATICOBJDATA_H
