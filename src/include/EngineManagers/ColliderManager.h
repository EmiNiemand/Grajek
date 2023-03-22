#ifndef OPENGLGP_COLLIDERMANAGER_H
#define OPENGLGP_COLLIDERMANAGER_H

#include "glm/matrix.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <memory>
#include <vector>
#include <map>

class Shader;
class GloomEngine;
class BoxCollider;

class ColliderManager {
private:
    unsigned int vao, vbo, ebo;
    std::vector<glm::vec3> vertices;
    std::vector<unsigned int> indices;
public:
    glm::vec3 debugColor = {0.0, 1.0, 0.0};

    std::shared_ptr<GloomEngine> gloomEngine;

    std::map<int, std::shared_ptr<BoxCollider>> boxColliders;
    std::shared_ptr<Shader> colliderDebugShader;

public:
    explicit ColliderManager(const std::shared_ptr<GloomEngine> &gloomEngine);
    virtual ~ColliderManager();

    void Update();
    void Free();

    void OnBoxColliderAdd();
    void RemoveBoxCollider(int componentId);
};


#endif //OPENGLGP_COLLIDERMANAGER_H
