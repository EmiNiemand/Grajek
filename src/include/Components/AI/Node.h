//
// Created by Adrian on 23.05.2023.
//

#ifndef GLOOMENGINE_NODE_H
#define GLOOMENGINE_NODE_H

#include "glm/matrix.hpp"
#include "glm/gtc/matrix_transform.hpp"

constexpr int STRAIGHT_MOVE_COST = 10;
constexpr int DIAGONAL_MOVE_COST = 14;

struct Node {
    Node* parent = nullptr;
    float fCost = 0, gCost = 0, hCost = 0;
    glm::ivec2 pos {};

    inline void CalculateFCost() {
        fCost = gCost + hCost;
    }

    // NOTE: octile distance (similar to Chebyshev, performs better than Manhattan or Euclidean)
    inline void CalculateHCost(const glm::ivec2& targetPosition) {
        int dx = std::fabs(targetPosition.x - pos.x), dy = std::fabs(targetPosition.y - pos.y);
        hCost = STRAIGHT_MOVE_COST * (dx + dy) + (DIAGONAL_MOVE_COST - 2 * STRAIGHT_MOVE_COST) * std::min(dx, dy);
    }

    inline void CalculateGCost(const glm::ivec2& position) {
        if ((position.x != 0 && position.y == 0) || (position.x == 0 && position.y != 0))
            gCost = parent->gCost + STRAIGHT_MOVE_COST;
        else
            gCost = parent->gCost + DIAGONAL_MOVE_COST;
    }
};

#endif //GLOOMENGINE_NODE_H
