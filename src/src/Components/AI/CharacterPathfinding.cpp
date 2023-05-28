//
// Created by Adrian on 13.05.2023.
//

#include "Components/AI/CharacterPathfinding.h"
#include "Components/AI/Node.h"

CharacterPathfinding::CharacterPathfinding()  {
    aiGrid = AIManager::GetInstance()->aiGrid;
    aiCellSize = AIManager::GetInstance()->aiCellSize;
    openList.reserve(1000);
    closedList.reserve(10000);
}

CharacterPathfinding::~CharacterPathfinding() = default;

inline const glm::vec3 CharacterPathfinding::GridToLocal(const glm::vec2& position) const {
    return {(position.x - AI_GRID_SIZE / 2.0f) * aiCellSize, 0, (position.y - AI_GRID_SIZE / 2.0f) * aiCellSize};
}

inline const glm::ivec2 CharacterPathfinding::LocalToGrid(const glm::vec2& position) const {
    return {position.x / aiCellSize + AI_GRID_SIZE / 2.0f, position.y / aiCellSize + AI_GRID_SIZE / 2.0f};
}

std::vector<glm::vec3>* CharacterPathfinding::FindNewPath(const glm::ivec2& currentPosition, const glm::ivec2& endTarget) {
    startGridPos = LocalToGrid(currentPosition);
    endGridPos = LocalToGrid(endTarget);

    // Possible implementation of limiting grid size by distance from start grid point to end
    // grid point, not implemented due to no/low performance gain
//    int distance = std::clamp((int)glm::distance(glm::vec2{startGridPos.x + 5, startGridPos.y + 5},
//                                             {endGridPos.x + 5, endGridPos.y + 5}), 0, AI_GRID_SIZE);

    currentNode = new Node();
    currentNode->pos = startGridPos;
    currentNode->gCost = 0;
    currentNode->CalculateHCost(endGridPos);

    openList[currentNode->pos.x * 100 + currentNode->pos.y] = currentNode;

    while (!openList.empty()) {
        if (currentNode->pos == endGridPos)
            break;

        for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
                if (i == 0 && j == 0)
                    continue;

                // Same as the comment before
//                gridIndex = {std::clamp(currentNode->pos.x + i, AI_GRID_SIZE / 2 - distance, AI_GRID_SIZE / 2 + distance),
//                             std::clamp(currentNode->pos.y + j, AI_GRID_SIZE / 2 - distance, AI_GRID_SIZE / 2 + distance)};

                gridIndex = {std::clamp(currentNode->pos.x + i, 0, AI_GRID_SIZE),
                             std::clamp(currentNode->pos.y + j, 0, AI_GRID_SIZE)};

                if (aiGrid[gridIndex.x][gridIndex.y] || closedList.contains(gridIndex.x * 100 + gridIndex.y))
                    continue;

                if (openList.contains(gridIndex.x * 100 + gridIndex.y)) {
                    node = openList[gridIndex.x * 100 + gridIndex.y];

                    if (node->gCost < currentNode->gCost) {
                        node->parent = currentNode;
                        node->CalculateHCost(endGridPos);
                        node->CalculateGCost({i, j});
                        node->CalculateFCost();
                    }
                } else {
                    node = new Node();
                    node->parent = currentNode;
                    node->pos = gridIndex;
                    node->CalculateHCost(endGridPos);
                    node->CalculateGCost({i, j});
                    node->CalculateFCost();
                    openList[gridIndex.x * 100 + gridIndex.y] = node;
                }
            }
        }

        maxCost = FLT_MAX;

        for (const auto &n: openList) {
            if (n.second->fCost < maxCost) {
                currentNode = n.second;
                maxCost = n.second->fCost;
            }
        }

        openList.erase(currentNode->pos.x * 100 + currentNode->pos.y);

        closedList[currentNode->pos.x * 100 + currentNode->pos.y] = currentNode;
    }

    if (currentNode == nullptr)
        return nullptr;

    size = 0;
    node = currentNode;

    while (node->parent != nullptr) {
        node = node->parent;
        ++size;
    }

    auto* squares = new std::vector<glm::vec3>;
    squares->reserve(size);

    while (currentNode->parent != nullptr) {
        squares->push_back(GridToLocal(currentNode->pos));
        currentNode = currentNode->parent;
    }

    spdlog::info("ol size: " + std::to_string(openList.size()));
    spdlog::info("cl size: " + std::to_string(closedList.size()));

    for (const auto& n : openList)
        delete n.second;

    openList.clear();

    for (const auto& n : closedList)
        delete n.second;

    closedList.clear();

    return squares;
}
