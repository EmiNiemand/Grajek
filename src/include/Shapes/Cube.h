//
// Created by masterktos on 23.03.23.
//

#ifndef GLOOMENGINE_CUBE_H
#define GLOOMENGINE_CUBE_H

#include "ShapeObject.h"
#include "glm/vec3.hpp"

namespace Shape {
    class Cube {
    public:
        static ShapeObject Get(glm::vec3 color = glm::vec3(1, 1, 1)) {
            ShapeObject shape;
            // Down
            shape.vertices.emplace_back();
            shape.vertices.back().position = glm::vec3(-1,-1,-1);   // DBL 0
            shape.vertices.back().texCoords = glm::vec2(1, 1);
            shape.vertices.emplace_back();
            shape.vertices.back().position = glm::vec3( 1,-1,-1);   // DBR 1
            shape.vertices.back().texCoords = glm::vec2(0, 1);
            shape.vertices.emplace_back();
            shape.vertices.back().position = glm::vec3( 1,-1, 1);   // DFR 2
            shape.vertices.back().texCoords = glm::vec2(1, 1);
            shape.vertices.emplace_back();
            shape.vertices.back().position = glm::vec3(-1,-1, 1);   // DFL 3
            shape.vertices.back().texCoords = glm::vec2(0, 1);
            // Up
            shape.vertices.emplace_back();
            shape.vertices.back().position = glm::vec3(-1, 1,-1);   // UBL 4
            shape.vertices.back().texCoords = glm::vec2(1, 0);
            shape.vertices.emplace_back();
            shape.vertices.back().position = glm::vec3( 1, 1,-1);   // UBR 5
            shape.vertices.back().texCoords = glm::vec2(0, 0);
            shape.vertices.emplace_back();
            shape.vertices.back().position = glm::vec3( 1, 1, 1);   // UFR 6
            shape.vertices.back().texCoords = glm::vec2(1, 0);
            shape.vertices.emplace_back();
            shape.vertices.back().position = glm::vec3(-1, 1, 1);   // UFL 7
            shape.vertices.back().texCoords = glm::vec2(0, 0);
            for(auto &vertex : shape.vertices)
                vertex.normal = color;
            shape.indices = {0, 1, 4,
                             1, 5, 4,

                             1, 2, 5,
                             2, 5, 6,

                             2, 3, 6,
                             3, 6, 7,

                             0, 3, 4,
                             3, 4, 7,

                             4, 5, 6,
                             4, 6, 7,

                             0, 1, 2,
                             1, 2, 3};
            return shape;
        }

        static std::shared_ptr<Mesh> GetWithNormals(const std::vector<Texture>& textures) {
            auto vertices = Get().vertices;
            ShapeObject shape;
            // Back face
            glm::vec3 backNormal = glm::vec3(0, 0, -1);
            shape.vertices.push_back(vertices[0]);
            shape.vertices.back().normal = backNormal;
            shape.vertices.push_back(vertices[1]);
            shape.vertices.back().normal = backNormal;
            shape.vertices.push_back(vertices[4]);
            shape.vertices.back().normal = backNormal;
            shape.vertices.push_back(vertices[5]);
            shape.vertices.back().normal = backNormal;
            //Right face
            glm::vec3 rightNormal = glm::vec3(-1, 0, 0);
            shape.vertices.push_back(vertices[1]);
            shape.vertices.back().normal = rightNormal;
            shape.vertices.push_back(vertices[2]);
            shape.vertices.back().normal = rightNormal;
            shape.vertices.push_back(vertices[5]);
            shape.vertices.back().normal = rightNormal;
            shape.vertices.push_back(vertices[6]);
            shape.vertices.back().normal = rightNormal;
            // Front face
            shape.vertices.push_back(vertices[2]);
            shape.vertices.back().normal = -backNormal;
            shape.vertices.push_back(vertices[3]);
            shape.vertices.back().normal = -backNormal;
            shape.vertices.push_back(vertices[6]);
            shape.vertices.back().normal = -backNormal;
            shape.vertices.push_back(vertices[7]);
            shape.vertices.back().normal = -backNormal;
            // Left face
            shape.vertices.push_back(vertices[0]);
            shape.vertices.back().normal = -rightNormal;
            shape.vertices.push_back(vertices[3]);
            shape.vertices.back().normal = -rightNormal;
            shape.vertices.push_back(vertices[4]);
            shape.vertices.back().normal = -rightNormal;
            shape.vertices.push_back(vertices[7]);
            shape.vertices.back().normal = -rightNormal;
            // Down face
            glm::vec3 downNormal = glm::vec3(0, -1, 0);
            shape.vertices.push_back(vertices[0]);
            shape.vertices.back().normal = downNormal;
            shape.vertices.push_back(vertices[1]);
            shape.vertices.back().normal = downNormal;
            shape.vertices.push_back(vertices[3]);
            shape.vertices.back().normal = downNormal;
            shape.vertices.push_back(vertices[2]);
            shape.vertices.back().normal = downNormal;
            // Up face
            shape.vertices.push_back(vertices[4]);
            shape.vertices.back().normal = -downNormal;
            shape.vertices.push_back(vertices[5]);
            shape.vertices.back().normal = -downNormal;
            shape.vertices.push_back(vertices[7]);
            shape.vertices.back().normal = -downNormal;
            shape.vertices.push_back(vertices[6]);
            shape.vertices.back().normal = -downNormal;

            shape.indices = {0, 1, 2,
                             1, 2, 3,

                             4, 5, 6,
                             5, 6, 7,

                             8, 9, 10,
                             9, 10, 11,

                             12, 13, 14,
                             13, 14, 15,

                             16, 17, 18,
                             17, 18, 19,

                             20, 21, 22,
                             21, 22, 23};
            return std::make_shared<Mesh>(shape.vertices, shape.indices, textures);
        }
    };
}


#endif //GLOOMENGINE_CUBE_H
