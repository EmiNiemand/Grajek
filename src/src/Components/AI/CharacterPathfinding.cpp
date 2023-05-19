//
// Created by Adrian on 13.05.2023.
//

#include "Components/AI/CharacterPathfinding.h"
#include "Components/PhysicsAndColliders/Rigidbody.h"
#include "GameObjectsAndPrefabs/GameObject.h"

inline static int GetIndex(const int x, const int y) {
    return x + y * AI_GRID_SIZE;
}

inline static glm::vec3 GridToLocal(const glm::vec2 position, float gridSize) {
    return {position.x - AI_GRID_SIZE / 2.0f * gridSize, 0, (position.y - AI_GRID_SIZE / 2.0f) * gridSize};
}

inline static glm::ivec2 LocalToGrid(const glm::vec2 position, const float gridSize) {
    return {-position.x / gridSize + AI_GRID_SIZE / 2.0f, position.y / gridSize + AI_GRID_SIZE / 2.0f};
}

const std::vector<glm::vec3> FindNewPath(const glm::vec2& currentPosition, const glm::vec2& endTarget) {
    auto gridPtr = AIManager::aiGrid;
    const float aiGridSize = AIManager::GetInstance()->aiGridSize;

    int gridIndex;

    std::vector<glm::vec3> path;
    std::vector<Node*> openList;
    std::vector<Node*> closedList;

    glm::ivec2 currentGridPos = LocalToGrid(currentPosition, aiGridSize);
    glm::ivec2 endTargetGridPos = LocalToGrid(endTarget, aiGridSize);

    Node* startNode = new Node();
    startNode->index = GetIndex(currentGridPos.x, currentGridPos.y);
    startNode->pos = currentGridPos;
    startNode->gCost = 0;
    startNode->CalculateHCost(currentGridPos, endTargetGridPos);

    Node* endNode = new Node();
    endNode->index = GetIndex(endTargetGridPos.x, endTargetGridPos.y);
    endNode->pos = endTargetGridPos;
    endNode->gCost = FLT_MAX;
    endNode->hCost = 0;

    Node* node, *previousNode = nullptr;
    Node* currentNode;
    openList.push_back(startNode);

    while (!openList.empty()) {
        currentNode = openList[0];
        for (int i = 1; i < openList.size(); i++) {
            if (openList[i]->GetFCost() < currentNode->GetFCost()) {
                currentNode = openList[i];
            }
        }

        std::erase(openList, currentNode);

        closedList.push_back(currentNode);

        if (currentNode->index == endNode->index)
            break;

        for (int x = -1; x <= 1; x++) {
            for (int y = -1; y <= 1; y++) {
                gridIndex = GetIndex(currentNode->pos.x + x, currentNode->pos.y + y);
                auto test = GridToLocal(currentNode->pos, aiGridSize);
                if (gridPtr[gridIndex])
                    spdlog::info(std::to_string(test.x) + ", " +  std::to_string(test.z));

                if (!gridPtr[gridIndex] && gridIndex != currentNode->index) {
                    for (const auto &n: closedList) {
                        if (n->index == gridIndex) {
                            previousNode = n;
                            break;
                        }
                    }

                    if (previousNode == nullptr) {
                        node = new Node();
                        node->index = gridIndex;
                        node->parent = currentNode;
                        node->pos = {currentNode->pos.x + x, currentNode->pos.y + y};
                        node->CalculateHCost(node->pos, endTargetGridPos);
                        node->CalculateGCost({x, y});

                        for (const auto &n: openList) {
                            if (n->index == gridIndex) {
                                previousNode = n;
                                break;
                            }
                        }

                        if (previousNode != nullptr) {
                            if (previousNode->gCost < currentNode->gCost) {
                                previousNode->parent = currentNode;
                                previousNode->CalculateGCost({x, y});
                            }
                        } else {
                            openList.push_back(node);
                        }
                    }

                    previousNode = nullptr;
                }
            }
        }
    }

    openList.clear();
    closedList.clear();

    while (currentNode != nullptr) {
        closedList.push_back(currentNode);
        currentNode = currentNode->parent;
    }

    for (int i = closedList.size() - 1; i > 0; i--) {
        path.push_back(GridToLocal(closedList[i]->pos, aiGridSize));
    }

    return path;
}
