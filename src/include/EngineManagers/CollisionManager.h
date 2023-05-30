#ifndef OPENGLGP_COLLIDERMANAGER_H
#define OPENGLGP_COLLIDERMANAGER_H

#include "glm/matrix.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <memory>
#include <thread>
#include <vector>
#include <unordered_map>

#define GRID_SIZE 100

class Shader;
class GloomEngine;
class BoxCollider;

class CollisionManager {
private:
    inline static CollisionManager* collisionManager;
    inline static unsigned int maxNumberOfThreads = std::thread::hardware_concurrency() / 2;

    std::vector<std::thread> threads;

    unsigned int vao, vbo, ebo;
    std::vector<glm::vec3> vertices;
    std::vector<unsigned int> indices;

public:
    glm::ivec2 playerPosition = glm::ivec2(0.0f);

#ifdef DEBUG
    glm::vec3 debugColor = {0.0, 1.0, 0.0};
    std::shared_ptr<Shader> colliderDebugShader;
#endif

    float gridSize = 10.0f;
    std::unordered_map<int, std::shared_ptr<BoxCollider>> grid[GRID_SIZE * GRID_SIZE];

public:
    CollisionManager(CollisionManager &other) = delete;
    void operator=(const CollisionManager&) = delete;
    virtual ~CollisionManager();

    static CollisionManager* GetInstance();

    void ManageCollision();
    void CheckCollision(int valueY);
#ifdef DEBUG
    void Draw();
#endif
    void Free();

#ifdef DEBUG
    void OnBoxCollidersChange();
#endif
    void RemoveBoxCollider(int componentId);
    void RemoveDynamicBoxCollider(const glm::vec3& position, int componentId);

private:
    explicit CollisionManager();
};


#endif //OPENGLGP_COLLIDERMANAGER_H
