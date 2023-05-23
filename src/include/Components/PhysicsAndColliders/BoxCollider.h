#ifndef OPENGLGP_BOXCOLLIDER_H
#define OPENGLGP_BOXCOLLIDER_H

#include "glm/matrix.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Components/Component.h"
#include <vector>
#include <unordered_map>
#include <cmath>

class GameObject;

class BoxCollider : public Component {
private:
    // Size of the box from center to direction
    glm::vec3 size{};
    glm::vec3 offset{};

    std::unordered_map<int, std::shared_ptr<GameObject>> collisionsBuffer;

public:
    bool isTrigger = false;

public:
    BoxCollider(const std::shared_ptr<GameObject> &parent, int id);
    ~BoxCollider() override;

    void Start() override;
    void FixedUpdate() override;
    void OnDestroy() override;

    void CheckCollision(const std::shared_ptr<BoxCollider>& other);
    std::vector<glm::vec3> GetBoxPoints();

    const glm::vec3 &GetSize() const;
    void SetSize(const glm::vec3 &size);

    const glm::vec3 &GetOffset() const;
    void SetOffset(const glm::vec3 &offset);

    glm::mat4 GetModelMatrix();
private:
    bool GetOBBCollision(const std::shared_ptr<BoxCollider>& other);
    void HandleCollision(const std::shared_ptr<BoxCollider>& other);

    void SetGridPoints();
    void SetCollidersGridPoints(const glm::ivec2* points);
    void SetAIGridPoints(const glm::ivec2* points);
};

#endif //OPENGLGP_BOXCOLLIDER_H
