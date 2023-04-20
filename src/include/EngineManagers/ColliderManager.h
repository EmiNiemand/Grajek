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

class ColliderManager {
private:
    inline static ColliderManager* colliderManager;

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
    std::unordered_map<int, std::unordered_map<int, std::unordered_map<int, std::shared_ptr<BoxCollider>>>> grid;

public:
    ColliderManager(ColliderManager &other) = delete;
    void operator=(const ColliderManager&) = delete;
    virtual ~ColliderManager();

    static ColliderManager* GetInstance();

    void ManageCollision();
#ifdef DEBUG
    void DrawColliders();
#endif
    void Free();

#ifdef DEBUG
    void OnBoxCollidersChange();
#endif
    void RemoveBoxCollider(int componentId);

private:
    explicit ColliderManager();
};


#endif //OPENGLGP_COLLIDERMANAGER_H
