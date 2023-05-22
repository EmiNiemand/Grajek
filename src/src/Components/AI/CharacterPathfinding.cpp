//
// Created by Adrian on 13.05.2023.
//

#include <algorithm>
#include "Components/AI/CharacterPathfinding.h"

CharacterPathfinding::CharacterPathfinding()  {
    aiGrid = AIManager::GetInstance()->aiGrid;
    aiGridSize = AIManager::GetInstance()->aiGridSize;
}

CharacterPathfinding::~CharacterPathfinding() = default;

inline const glm::vec3 CharacterPathfinding::GridToLocal(const glm::vec2& position) const {
    return {(position.x - AI_GRID_SIZE / 2.0f) * aiGridSize, 0, (position.y - AI_GRID_SIZE / 2.0f) * aiGridSize};
}

inline const glm::ivec2 CharacterPathfinding::LocalToGrid(const glm::vec2& position) const {
    return {position.x / aiGridSize + AI_GRID_SIZE / 2.0f, position.y / aiGridSize + AI_GRID_SIZE / 2.0f};
}

const std::vector<glm::vec3> CharacterPathfinding::FindNewPath(const glm::ivec2& currentPosition, const glm::ivec2& endTarget) {
    startGridPos = LocalToGrid(currentPosition);
    endGridPos = LocalToGrid(endTarget);

    currentNode = std::make_shared<Node>();
    currentNode->pos = startGridPos;
    currentNode->gCost = 0;
    currentNode->CalculateHCost(endGridPos);

    openList.insert({startGridPos.x * 100 + startGridPos.y, currentNode});

    while (!openList.empty()) {
        if (currentNode->pos == endGridPos)
            break;

        for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
                if (i == 0 && j == 0)
                    continue;

                gridIndex = {std::clamp(currentNode->pos.x + i, 0, AI_GRID_SIZE),
                             std::clamp(currentNode->pos.y + j, 0, AI_GRID_SIZE)};

                if (aiGrid[gridIndex.x][gridIndex.y] || closedList.contains(gridIndex.x * 100 + gridIndex.y))
                    continue;

                if (openList.contains(gridIndex.x * 100 + gridIndex.y)) {
                    node = openList.at(gridIndex.x * 100 + gridIndex.y);

                    if (node->gCost < currentNode->gCost) {
                        node->parent = currentNode;
                        node->CalculateHCost(endGridPos);
                        node->CalculateGCost({i, j});
                        node->CalculateFCost();
                    }
                } else {
                    node = std::make_shared<Node>();
                    node->parent = currentNode;
                    node->pos = gridIndex;
                    node->CalculateHCost(endGridPos);
                    node->CalculateGCost({i, j});
                    node->CalculateFCost();
                    openList.insert({gridIndex.x * 100 + gridIndex.y, node});
                }
            }
        }

        maxfCost = FLT_MAX;

        for (const auto &n: openList) {
            if (n.second->fCost < maxfCost) {
                currentNode = n.second;
                maxfCost = n.second->fCost;
            }
        }

        openList.erase(currentNode->pos.x * 100 + currentNode->pos.y);

        closedList.insert({currentNode->pos.x * 100 + currentNode->pos.y, currentNode});
    }


    std::vector<glm::vec3> squares;

    if (currentNode == nullptr)
        return squares;

    while (currentNode->parent != nullptr) {
        squares.push_back(GridToLocal(currentNode->pos));
        currentNode = currentNode->parent;
    }

    openList.clear();

    closedList.clear();

    return squares;
}
