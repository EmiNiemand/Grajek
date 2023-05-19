#ifndef OPENGLGP_CHARACTERPATHFINDING_H
#define OPENGLGP_CHARACTERPATHFINDING_H

#include "glm/matrix.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Components/PhysicsAndColliders/BoxCollider.h"
#include "EngineManagers/AIManager.h"
#include <vector>
#include <cmath>

constexpr int STRAIGHT_MOVE_COST = 10;
constexpr int DIAGONAL_MOVE_COST = 14;

struct Node {
    Node* parent;
    int index;
    float gCost, hCost;
    glm::ivec2 pos;

    [[nodiscard]] const float GetFCost() const {
        return gCost + hCost;
    }

    void CalculateHCost(const glm::vec2 position, const glm::vec2 targetPosition) {
        hCost = glm::distance(position, targetPosition);
    }

    void CalculateGCost(const glm::vec2 position) {
        if (position.x == 1 && position.y == 0 || position.x == -1 && position.y == 0 ||
            position.x == 0 && position.y == 1 || position.x == 0 && position.y == -1)
            gCost = parent->gCost + STRAIGHT_MOVE_COST;
        else
            gCost = parent->gCost + DIAGONAL_MOVE_COST;
    }
};

const std::vector<glm::vec3> FindNewPath(const glm::vec2& currentPosition, const glm::vec2& endTarget);
inline static int GetIndex(int x, int y);
inline static glm::vec3 GridToLocal(glm::vec2 position, float gridSize);
inline static glm::ivec2 LocalToGrid(glm::vec2 position, float gridSize);

#endif //OPENGLGP_CHARACTERPATHFINDING_H
