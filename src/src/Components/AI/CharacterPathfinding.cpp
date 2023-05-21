//
// Created by Adrian on 13.05.2023.
//

#include "Components/AI/CharacterPathfinding.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/PhysicsAndColliders/Rigidbody.h"
#include "Components/PhysicsAndColliders/BoxCollider.h"

CharacterPathfinding::CharacterPathfinding() = default;

CharacterPathfinding::~CharacterPathfinding() = default;

inline const glm::vec3 CharacterPathfinding::GridToLocal(const glm::vec2& position) const {
    return {(position.x - AI_GRID_SIZE / 2.0f) * aiGridSize, 0, (position.y - AI_GRID_SIZE / 2.0f) * aiGridSize};
}

inline const glm::ivec2 CharacterPathfinding::LocalToGrid(const glm::vec2& position) const {
    return {position.x / aiGridSize + AI_GRID_SIZE / 2.0f, position.y / aiGridSize + AI_GRID_SIZE / 2.0f};
}

std::vector<glm::vec3>* CharacterPathfinding::FindNewPath(const glm::ivec2& currentPosition, const glm::ivec2& endTarget) {
    const glm::ivec2 startGridPos = LocalToGrid(currentPosition);
    const glm::ivec2 endGridPos = LocalToGrid(endTarget);
    glm::ivec2 gridIndex;

    std::unordered_map<int, Node*> openList;
    std::unordered_map<int, Node*> closedList;

    Node *currentNode = new Node();
    currentNode->pos = startGridPos;
    currentNode->gCost = 0;
    currentNode->CalculateHCost(endGridPos);

    Node *endNode = new Node();
    endNode->pos = endGridPos;
    endNode->fCost = 100000;

    Node *node = nullptr;

    openList.insert({startGridPos.x * 100 + startGridPos.y, currentNode});

    while (!openList.empty()) {
        openList.erase(currentNode->pos.x * 100 + currentNode->pos.y);

        closedList.insert({currentNode->pos.x * 100 + currentNode->pos.y, currentNode});

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
                    node = new Node();
                    node->parent = currentNode;
                    node->pos = gridIndex;
                    node->CalculateHCost(endGridPos);
                    node->CalculateGCost({i, j});
                    node->CalculateFCost();
                    openList.insert({gridIndex.x * 100 + gridIndex.y, node});
                }
            }
        }

        currentNode = endNode;

        for (const auto &n: openList) {
            if (n.second->fCost < currentNode->fCost)
                currentNode = n.second;
        }
    }

    std::vector<glm::vec3> squares;

    while (currentNode != nullptr) {
        squares.push_back(GridToLocal(currentNode->pos));
        currentNode = currentNode->parent;
    }

    std::reverse(squares.begin(), squares.end());

    auto* path = new std::vector<glm::vec3>(squares.size());

    std::move(squares.begin(), squares.end(), path->begin());

    for (auto &n: openList)
        delete n.second;

    openList.clear();

    for (auto &n: closedList)
        delete n.second;

    closedList.clear();

    delete endNode;

    return path;
}
