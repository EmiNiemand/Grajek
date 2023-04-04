//
// Created by masterktos on 03.04.23.
//

#ifndef GLOOMENGINE_PLANE_H
#define GLOOMENGINE_PLANE_H

#include "ShapeObject.h"
#include "glm/vec3.hpp"

namespace Shape {
    class Plane {
        // Plane's front is towards Z plane
        static ShapeObject Get() {
            ShapeObject shape;
            Vertex vertex{};
            // left bottom
            vertex = shape.vertices.emplace_back();
            vertex.position = glm::vec3(0.0f);
            vertex.texCoords = glm::vec2(0.0f, 0.0f);
            // left top
            vertex = shape.vertices.emplace_back();
            vertex.position = glm::vec3( 0.0f);
            vertex.texCoords = glm::vec2(0.0f, 1.0f);
            // right bottom
            vertex = shape.vertices.emplace_back();
            vertex.position = glm::vec3(0.0f);
            vertex.texCoords = glm::vec2(1.0f, 0.0f);
            // right top
            vertex = shape.vertices.emplace_back();
            vertex.position = glm::vec3(0.0f);
            vertex.texCoords = glm::vec2(1.0f, 1.0f);

            shape.indices = {0, 1, 3,
                             0, 2, 3};

            return shape;
        }
        static std::shared_ptr<Mesh> GetWithNormals(const std::vector<Texture>& textures) {
            auto shape = Get();
            // all normals are directed in the same way anyway
            glm::vec3 normal = glm::vec3(0, 0, 1);
            for(auto vertex : shape.vertices) {
                vertex.normal = normal;
            }
            //indices stay the same, since it's still just one face

            return std::make_shared<Mesh>(shape.vertices, shape.indices, textures);
        }
    };
}

#endif //GLOOMENGINE_PLANE_H
