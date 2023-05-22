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
    std::shared_ptr<Node> parent = nullptr;
    float gCost, hCost, fCost;
    glm::ivec2 pos {};

    inline void CalculateFCost() {
        fCost = gCost + hCost;
    }

    inline void CalculateHCost(const glm::vec2& targetPosition) {
        hCost = glm::distance({pos.x, pos.y}, targetPosition);
    }

    inline void CalculateGCost(const glm::vec2& position) {
        if ((position.x != 0 && position.y == 0) || (position.x == 0 && position.y != 0))
            gCost = parent->gCost + STRAIGHT_MOVE_COST;
        else
            gCost = parent->gCost + DIAGONAL_MOVE_COST;
    }
};

class CharacterPathfinding {
    const bool (*aiGrid)[100];
    float aiGridSize;
    glm::ivec2 startGridPos {};
    glm::ivec2 endGridPos {};
    glm::ivec2 gridIndex {};
    std::unordered_map<int, std::shared_ptr<Node>> openList;
    std::unordered_map<int, std::shared_ptr<Node>> closedList;
    std::shared_ptr<Node> currentNode = nullptr;
    std::shared_ptr<Node> node = nullptr;
    float maxfCost = FLT_MAX;

    [[nodiscard]] inline const glm::vec3 GridToLocal(const glm::vec2& position) const;
    [[nodiscard]] inline const glm::ivec2 LocalToGrid(const glm::vec2& position) const;

public:

    explicit CharacterPathfinding();
    ~CharacterPathfinding();

    const std::vector<glm::vec3> FindNewPath(const glm::ivec2& currentPosition, const glm::ivec2& endTarget);
};

#endif //OPENGLGP_CHARACTERPATHFINDING_H
