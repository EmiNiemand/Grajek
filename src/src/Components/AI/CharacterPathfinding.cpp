//
// Created by Adrian on 13.05.2023.
//

#include "EngineManagers/AIManager.h"
#include "Components/AI/CharacterPathfinding.h"
#include "Components/AI/Node.h"

CharacterPathfinding::CharacterPathfinding()  {
    aiGrid = AIManager::GetInstance()->aiGrid;
    openList.reserve(1000);
    closedList.reserve(10000);
}

CharacterPathfinding::~CharacterPathfinding() = default;

/**
 * @annotation
 * Translates grid position to local.
 * @param position - grid position
 * @returns glm::vec3 - local position
 */
inline const glm::vec3 CharacterPathfinding::GridToLocal(const glm::vec2& position) const {
    return {(position.x - AI_GRID_SIZE / 2.0f) * AI_CELL_SIZE, 0, (position.y - AI_GRID_SIZE / 2.0f) * AI_CELL_SIZE};
}

/**
 * @annotation
 * Translates local position to grid.
 * @param position - local position
 * @returns glm::ivec2 - grid position
 */
inline const glm::ivec2 CharacterPathfinding::LocalToGrid(const glm::vec2& position) const {
    return {position.x / AI_CELL_SIZE + AI_GRID_SIZE / 2.0f, position.y / AI_CELL_SIZE + AI_GRID_SIZE / 2.0f};
}

/**
 * @annotation
 * Calculates new path with the A* algorithm.
 * @param currentPosition - start/current position
 * @param endTarget - end position
 * @returns std::vector\<glm::vec3\>* - pointer to vector of points
 */
std::vector<glm::vec3>* CharacterPathfinding::FindNewPath(const glm::ivec2& currentPosition, const glm::ivec2& endTarget) {
    startGridPos = LocalToGrid(currentPosition);
    endGridPos = LocalToGrid(endTarget);

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

                gridIndex = {std::clamp(currentNode->pos.x + i, 0, AI_GRID_SIZE),
                             std::clamp(currentNode->pos.y + j, 0, AI_GRID_SIZE)};

                if (aiGrid[gridIndex.x + gridIndex.y * AI_GRID_SIZE] || closedList.contains(gridIndex.x * 100 + gridIndex.y))
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

    for (const auto& n : openList)
        delete n.second;

    openList.clear();

    for (const auto& n : closedList)
        delete n.second;

    closedList.clear();

    return squares;
}
