#ifndef OPENGLGP_BOXCOLLIDER_H
#define OPENGLGP_BOXCOLLIDER_H

#include "glm/matrix.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Components/Component.h"
#include <vector>
#include <cmath>

class GameObject;

class BoxCollider : public Component, public std::enable_shared_from_this<BoxCollider> {
private:
    // Size of the box from center to direction
    glm::vec3 size{};
    glm::vec3 offset{};

public:
    BoxCollider(const std::shared_ptr<GameObject> &parent, int id);
    ~BoxCollider() override;

    void HandleCollision(const std::shared_ptr<BoxCollider>& other);
    std::vector<glm::vec3> GetBoxPoints();

    const glm::vec3 &GetSize() const;
    void SetSize(const glm::vec3 &size);

    const glm::vec3 &GetOffset() const;
    void SetOffset(const glm::vec3 &offset);

    glm::mat4 GetModelMatrix();
    bool GetOBBCollision(const std::shared_ptr<BoxCollider> &other);
};

#endif //OPENGLGP_BOXCOLLIDER_H
