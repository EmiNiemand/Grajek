#ifndef OPENGLGP_ENGINECOLLIDERS_H
#define OPENGLGP_ENGINECOLLIDERS_H

#include "glm/matrix.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <memory>
#include <vector>
#include <map>

class Shader;
class GloomEngine;
class BoxCollider;

class EngineColliders {
private:
    unsigned int vao, vbo, ebo;
    std::vector<glm::vec3> vertices;
    std::vector<unsigned int> indices;
public:
    bool isDebugOn;
    glm::vec3 debugColor = {0.0, 1.0, 0.0};

    std::shared_ptr<GloomEngine> gloomEngine;

    std::map<int, std::shared_ptr<BoxCollider>> boxColliders;
    std::shared_ptr<Shader> colliderDebugShader;

public:
    explicit EngineColliders(const std::shared_ptr<GloomEngine> &gloomEngine, bool isDebugOn = false);
    virtual ~EngineColliders();

    void Update();
    void Free();

    void OnBoxColliderAdd();
    void RemoveBoxCollider(int componentId);
};


#endif //OPENGLGP_ENGINECOLLIDERS_H
