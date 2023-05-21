#ifndef OPENGLGP_CHARACTERPATHFINDING_H
#define OPENGLGP_CHARACTERPATHFINDING_H

#include "glm/matrix.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <vector>
#include <cmath>

constexpr int AI_GRID_SIZE = 200;
constexpr int STRAIGHT_MOVE_COST = 10;
constexpr int DIAGONAL_MOVE_COST = 14;

struct Node {
    Node* parent = nullptr;
    float gCost, hCost, fCost;
    glm::ivec2 pos {};

    inline void CalculateFCost() {
        fCost = gCost + hCost;
    }

    inline void CalculateHCost(const glm::vec2& targetPosition) {
        hCost = glm::distance({pos.x, pos.y}, targetPosition);
    }

    inline void CalculateGCost(const glm::vec2& position) {
        if (position.x == 1 && position.y == 0 || position.x == -1 && position.y == 0 ||
            position.x == 0 && position.y == 1 || position.x == 0 && position.y == -1) {
            if (parent == nullptr)
                gCost = STRAIGHT_MOVE_COST;
            else
                gCost = parent->gCost + STRAIGHT_MOVE_COST;
        } else {
            if (parent == nullptr)
                gCost = DIAGONAL_MOVE_COST;
            else
                gCost = parent->gCost + DIAGONAL_MOVE_COST;
        }
    }
};

class CharacterPathfinding {

public:
    const float aiGridSize = 2.0f;
    bool aiGrid[AI_GRID_SIZE][AI_GRID_SIZE] = {};

    explicit CharacterPathfinding();
    virtual ~CharacterPathfinding();

    [[nodiscard]] inline const glm::vec3 GridToLocal(const glm::vec2& position) const;
    [[nodiscard]] inline const glm::ivec2 LocalToGrid(const glm::vec2& position) const;
    std::vector<glm::vec3>* FindNewPath(const glm::ivec2& currentPosition, const glm::ivec2& endTarget);
};

#endif //OPENGLGP_CHARACTERPATHFINDING_H
