#ifndef OPENGLGP_COLLIDERMANAGER_H
#define OPENGLGP_COLLIDERMANAGER_H

#include "glm/matrix.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <memory>
#include <vector>
#include <unordered_map>

class Shader;
class GloomEngine;
class BoxCollider;

class CollisionManager {
private:
    inline static CollisionManager* colliderManager;

    unsigned int vao, vbo, ebo;
    std::vector<glm::vec3> vertices;
    std::vector<unsigned int> indices;

public:
#ifdef DEBUG
    glm::ivec2 playerPosition = glm::ivec2(0.0f);
    glm::vec3 debugColor = {0.0, 1.0, 0.0};
    std::shared_ptr<Shader> colliderDebugShader;
#endif

    float gridSize = 10.0f;
    std::unordered_map<int, std::unordered_map<int, std::unordered_map<uint32_t, std::shared_ptr<BoxCollider>>>> grid;

public:
    CollisionManager(CollisionManager &other) = delete;
    void operator=(const CollisionManager&) = delete;
    virtual ~CollisionManager();

    static CollisionManager* GetInstance();

    void ManageCollision();
#ifdef DEBUG
    void DrawColliders();
#endif
    void Free();

#ifdef DEBUG
    void OnBoxCollidersChange();
#endif
    void RemoveBoxCollider(uint32_t componentId);

private:
    explicit CollisionManager();
};


#endif //OPENGLGP_COLLIDERMANAGER_H
