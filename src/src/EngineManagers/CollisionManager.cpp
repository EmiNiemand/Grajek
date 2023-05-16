#include "EngineManagers/CollisionManager.h"
#include "GloomEngine.h"
#include "EngineManagers/RendererManager.h"
#include "LowLevelClasses/Shader.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/Renderers/Camera.h"
#include "Components/PhysicsAndColliders/BoxCollider.h"
#include "Components/PhysicsAndColliders/Rigidbody.h"

#ifdef DEBUG
#include <tracy/Tracy.hpp>
#endif

CollisionManager::CollisionManager() {
#ifdef DEBUG
    colliderDebugShader = std::make_shared<Shader>("colliderDebug.vert", "colliderDebug.frag");
#endif
    // create buffers/arrays
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
}

CollisionManager::~CollisionManager() = default;

CollisionManager* CollisionManager::GetInstance() {
    if (colliderManager == nullptr) {
        colliderManager = new CollisionManager();
    }

    return colliderManager;
}


void CollisionManager::ManageCollision() {
    int gridPos;

    glm::vec3 playerPos = GloomEngine::GetInstance()->FindGameObjectWithName("Player")->transform->GetGlobalPosition();
    playerPosition = glm::ivec2((int)(playerPos.x / gridSize) + GRID_SIZE / 2, (int)(playerPos.z / gridSize) + GRID_SIZE / 2);

    // Handle collision
    for (int x = -5; x <= 5; x++) {
        for (int y = -5; y <= 5; y++) {
            gridPos = (playerPosition.x + x) + (playerPosition.y + y) * GRID_SIZE;
            for (const auto& box : grid[gridPos]) {
                if (!box.second->GetParent()->GetComponent<Rigidbody>() && !box.second->isTrigger) continue;

                for (const auto& box2 : grid[gridPos]) {
                    if (box.second == box2.second) continue;

                    glm::vec3 boxPosition = glm::vec3(box.second->GetModelMatrix() * glm::vec4(0,0,0,1));
                    glm::vec3 box2Position = glm::vec3(box2.second->GetModelMatrix() * glm::vec4(0,0,0,1));
                    float distance = glm::length(glm::vec2(box2Position.x, box2Position.z) - glm::vec2(boxPosition.x, boxPosition.z));

                    glm::vec3 boxScale = box.second->GetSize() * box.second->GetParent()->transform->GetGlobalScale();
                    float boxSizeLength = glm::length(glm::vec3(boxScale.x, 0, boxScale.z));

                    glm::vec3 box2Scale = box2.second->GetSize() * box2.second->GetParent()->transform->GetGlobalScale();
                    float box2SizeLength = glm::length(glm::vec3(box2Scale.x, 0, box2Scale.z));

                    if (distance >= boxSizeLength + box2SizeLength) continue;

                    box.second->CheckCollision(box2.second);
                }
            }
        }
    }
}

#ifdef DEBUG
void CollisionManager::Draw() {
    OnBoxCollidersChange();

    colliderDebugShader->Activate();
    colliderDebugShader->SetVec3("color", debugColor);
    colliderDebugShader->SetMat4("projection", RendererManager::GetInstance()->projection);
    colliderDebugShader->SetMat4("view", Camera::activeCamera->GetComponent<Camera>()->GetViewMatrix());

    for (auto&& box : grid[playerPosition.x + playerPosition.y * GRID_SIZE]) {
        colliderDebugShader->SetMat4("model", box.second->GetModelMatrix());
        glBindVertexArray(vao);
        glDrawElements(GL_LINES, (int)indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}
#endif

void CollisionManager::Free() {
#ifdef DEBUG
    colliderDebugShader->Delete();
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
#endif

    for (int i = 0; i < GRID_SIZE * GRID_SIZE; ++i) {
        grid[i].clear();
    }
}

void CollisionManager::RemoveBoxCollider(int componentId) {
#ifdef DEBUG
    ZoneScopedNC("RBC", 0x0339fc);
#endif
    for (int i = 0; i < GRID_SIZE * GRID_SIZE; i++) {
        if (grid[i].contains(componentId)) grid[i].erase(componentId);
    }
}

void CollisionManager::RemoveDynamicBoxCollider(const glm::vec3& position, int componentId) {
#ifdef DEBUG
    ZoneScopedNC("RDBC", 0x0339fc);
#endif
    glm::ivec2 gridPos = glm::ivec2((int)(position.x / gridSize) + GRID_SIZE / 2, (int)(position.z / gridSize) + GRID_SIZE / 2);

    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            int newGridPos = (gridPos.x + x) + (gridPos.y + y) * GRID_SIZE;
            if (grid[newGridPos].contains(componentId)) grid[newGridPos].erase(componentId);
        }
    }
}

#ifdef DEBUG
void CollisionManager::OnBoxCollidersChange() {
    vertices.clear();
    indices.clear();
    int i = 0;

    for (auto&& col : grid[playerPosition.x + playerPosition.y * GRID_SIZE]) {
        for (auto&& point : col.second->GetBoxPoints()) {
            vertices.push_back(point);
        }
        // UP LEFT
        indices.push_back(i * 8 + 0);
        indices.push_back(i * 8 + 1);
        indices.push_back(i * 8 + 2);

        // UP RIGHT
        indices.push_back(i * 8 + 1);
        indices.push_back(i * 8 + 3);
        indices.push_back(i * 8 + 2);

        // FRONT LEFT
        indices.push_back(i * 8 + 0);
        indices.push_back(i * 8 + 6);
        indices.push_back(i * 8 + 4);

        // FRONT RIGHT
        indices.push_back(i * 8 + 0);
        indices.push_back(i * 8 + 2);
        indices.push_back(i * 8 + 6);

        // LEFT BACK
        indices.push_back(i * 8 + 1);
        indices.push_back(i * 8 + 4);
        indices.push_back(i * 8 + 5);

        // LEFT FRONT
        indices.push_back(i * 8 + 1);
        indices.push_back(i * 8 + 0);
        indices.push_back(i * 8 + 4);

        // RIGHT BACK
        indices.push_back(i * 8 + 3);
        indices.push_back(i * 8 + 7);
        indices.push_back(i * 8 + 6);

        // RIGHT FRONT
        indices.push_back(i * 8 + 3);
        indices.push_back(i * 8 + 6);
        indices.push_back(i * 8 + 2);

        // DOWN BACK
        indices.push_back(i * 8 + 4);
        indices.push_back(i * 8 + 5);
        indices.push_back(i * 8 + 7);

        // DOWN FRONT
        indices.push_back(i * 8 + 4);
        indices.push_back(i * 8 + 7);
        indices.push_back(i * 8 + 6);

        // BACK DOWN
        indices.push_back(i * 8 + 5);
        indices.push_back(i * 8 + 1);
        indices.push_back(i * 8 + 7);


        // BACK UP
        indices.push_back(i * 8 + 1);
        indices.push_back(i * 8 + 3);
        indices.push_back(i * 8 + 7);

        i++;
    }

    if (vertices.empty() || indices.empty()) return;
    glBindVertexArray(vao);
    // load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // A great thing about structs is that their memory layout is sequential for all its items.
    // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
    // again translates to 3/2 floats which translates to a byte array.
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    // set the vertex attribute pointers
    // vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glBindVertexArray(0);
}
#endif