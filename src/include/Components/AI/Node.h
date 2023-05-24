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

// NOTE: Manhattan distance (performs better than Euclidean)
    inline void CalculateHCost(const glm::ivec2& targetPosition) {
        hCost = std::fabs(targetPosition.x - pos.x) + std::fabs(targetPosition.y - pos.y);
    }

    inline void CalculateGCost(const glm::ivec2& position) {
        if ((position.x != 0 && position.y == 0) || (position.x == 0 && position.y != 0))
            gCost = parent->gCost + STRAIGHT_MOVE_COST;
        else
            gCost = parent->gCost + DIAGONAL_MOVE_COST;
    }
};

#endif //GLOOMENGINE_NODE_H
