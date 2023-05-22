#ifndef OPENGLGP_CHARACTERPATHFINDING_H
#define OPENGLGP_CHARACTERPATHFINDING_H

#include "glm/matrix.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "EngineManagers/AIManager.h"
#include "Components/Component.h"
#include <vector>
#include <cmath>
#include <unordered_map>

constexpr int STRAIGHT_MOVE_COST = 10;
constexpr int DIAGONAL_MOVE_COST = 14;

struct Node {
    Node* parent = nullptr;
    float gCost, hCost, fCost;
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

class CharacterPathfinding {
    const bool (*aiGrid)[100];
    float aiCellSize;
    glm::ivec2 startGridPos {};
    glm::ivec2 endGridPos {};
    glm::ivec2 gridIndex {};
    std::unordered_map<int, Node*> openList;
    std::unordered_map<int, Node*> closedList;
    Node* currentNode = nullptr;
    Node* node = nullptr;
    int size = 0;
    float maxCost = FLT_MAX;

    [[nodiscard]] inline const glm::vec3 GridToLocal(const glm::vec2& position) const;
    [[nodiscard]] inline const glm::ivec2 LocalToGrid(const glm::vec2& position) const;

public:
    explicit CharacterPathfinding();
    ~CharacterPathfinding();

    std::vector<glm::vec3>* FindNewPath(const glm::ivec2& currentPosition, const glm::ivec2& endTarget);
};

#endif //OPENGLGP_CHARACTERPATHFINDING_H
