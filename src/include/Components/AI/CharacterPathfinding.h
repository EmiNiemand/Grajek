#ifndef OPENGLGP_CHARACTERPATHFINDING_H
#define OPENGLGP_CHARACTERPATHFINDING_H

#include "glm/matrix.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "spdlog/spdlog.h"

#include "EngineManagers/AIManager.h"
#include <cmath>
#include <unordered_map>
#include <algorithm>

struct Node;

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
