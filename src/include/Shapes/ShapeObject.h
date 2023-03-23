//
// Created by masterktos on 23.03.23.
//

#ifndef GLOOMENGINE_SHAPEOBJECT_H
#define GLOOMENGINE_SHAPEOBJECT_H

#include <vector>
#include "LowLevelClasses/Mesh.h"

namespace Shape {
    struct ShapeObject {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
    };
}
#endif //GLOOMENGINE_SHAPEOBJECT_H
