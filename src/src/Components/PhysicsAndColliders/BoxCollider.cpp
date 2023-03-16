#include "include/Components/PhysicsAndColliders/BoxCollider.h"
#include "include/GloomEngine.h"
#include "include/GameObjectsAndPrefabs/GameObject.h"
#include "include/EngineComponents/EngineColliders.h"
#include "include/Components/PhysicsAndColliders/Rigidbody.h"

BoxCollider::BoxCollider(const std::shared_ptr<GloomEngine> &gloomEngine, const std::shared_ptr<GameObject> &parent, int id)
        : Component(gloomEngine, parent, id) {
    name = ComponentNames::BOXCOLLIDER;
    size = {1.0f, 1.0f, 1.0f};
    offset = {0.0f, 0.0f, 0.0f};
}

BoxCollider::~BoxCollider() {}

void BoxCollider::HandleCollision(std::shared_ptr<BoxCollider> other) {
    glm::vec3 rotation = parent->transform->GetLocalRotation();
    glm::vec3 otherRotation = other->parent->transform->GetLocalRotation();

    int deg = (int)std::round(rotation.x + rotation.y + rotation.z);
    int otherDeg = (int)std::round(otherRotation.x + otherRotation.y + otherRotation.z);

    glm::vec3 minBoxPos = GetModelMatrix() * glm::vec4(-1, -1, -1, 1);
    glm::vec3 maxBoxPos = GetModelMatrix() * glm::vec4(1, 1, 1, 1);

    glm::vec3 minOtherPos = other->GetModelMatrix() * glm::vec4(-1, -1, -1, 1);
    glm::vec3 maxOtherPos = other->GetModelMatrix() * glm::vec4(1, 1, 1, 1);
    // Collision checker for not rotated cubes AABB method
    if (deg % 90 == 0 && otherDeg % 90 == 0){
        if (maxBoxPos.x >= minOtherPos.x && maxBoxPos.x <= maxOtherPos.x &&
            maxBoxPos.y >= minOtherPos.y && maxBoxPos.y <= maxOtherPos.y &&
            maxBoxPos.z >= minOtherPos.z && maxBoxPos.z <= maxOtherPos.z ||
            minBoxPos.x >= minOtherPos.x && minBoxPos.x <= maxOtherPos.x &&
            minBoxPos.y >= minOtherPos.y && minBoxPos.y <= maxOtherPos.y &&
            minBoxPos.z >= minOtherPos.z && minBoxPos.z <= maxOtherPos.z) {

            const glm::mat4 transformX = glm::rotate(glm::mat4(1.0f), glm::radians(otherRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
            const glm::mat4 transformY = glm::rotate(glm::mat4(1.0f), glm::radians(otherRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
            const glm::mat4 transformZ = glm::rotate(glm::mat4(1.0f), glm::radians(otherRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

            // Y * X * Z
            const glm::mat4 rotationMatrix = transformY * transformX * transformZ;

            glm::vec3 vectors[6];
            vectors[0] = glm::vec3(rotationMatrix * glm::vec4(1,0,0,1));
            vectors[1] = glm::vec3(rotationMatrix * glm::vec4(0,1,0,1));
            vectors[2] = glm::vec3(rotationMatrix * glm::vec4(0,0,1,1));
            vectors[3] = -vectors[0];
            vectors[4] = -vectors[1];
            vectors[5] = -vectors[2];

            glm::vec3 position = parent->transform->GetLocalPosition() + offset  * parent->transform->GetLocalScale();
            glm::vec3 otherPosition = other->parent->transform->GetLocalPosition() + other->offset  * other->parent->transform->GetLocalScale();
            glm::vec3 difPosition = position - otherPosition;

            glm::vec3 closestVector = glm::vec3(0, 0, 0);
            float closestDistance = glm::distance(otherPosition + glm::normalize(vectors[0]), position);

            for(auto vector : vectors) {
                float distance = glm::distance(otherPosition + glm::normalize(vector), position);
                if (distance < closestDistance) {
                    closestDistance = distance;
                    closestVector = vector;
                    continue;
                }
                if (distance == closestDistance) {
                    glm::vec3 point = otherPosition + glm::normalize(vector) * difPosition;
                    if(!(point.x >= minOtherPos.x &&
                       point.x <= maxOtherPos.x &&
                       point.y >= minOtherPos.y &&
                       point.y <= maxOtherPos.y &&
                       point.z >= minOtherPos.z &&
                       point.z <= maxOtherPos.z)) {
                        closestDistance = distance;
                        closestVector += vector;
                    }
                }
            }

            if (parent->GetComponent<Rigidbody>() != nullptr) {
                glm::vec3 velocity = -parent->GetComponent<Rigidbody>()->velocity;
                velocity = glm::normalize(closestVector) * velocity;
                parent->GetComponent<Rigidbody>()->AddForce(velocity, ForceMode::Impulse);
            }
        }
        return;
    }

    // OBB
    if (GetOBBCollision(other)) {
        const glm::mat4 transformX = glm::rotate(glm::mat4(1.0f), glm::radians(otherRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        const glm::mat4 transformY = glm::rotate(glm::mat4(1.0f), glm::radians(otherRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        const glm::mat4 transformZ = glm::rotate(glm::mat4(1.0f), glm::radians(otherRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

        // Y * X * Z
        const glm::mat4 rotationMatrix = transformY * transformX * transformZ;

        glm::vec3 vectors[6];
        vectors[0] = glm::vec3(rotationMatrix * glm::vec4(1,0,0,1));
        vectors[1] = glm::vec3(rotationMatrix * glm::vec4(0,1,0,1));
        vectors[2] = glm::vec3(rotationMatrix * glm::vec4(0,0,1,1));
        vectors[3] = -vectors[0];
        vectors[4] = -vectors[1];
        vectors[5] = -vectors[2];

        glm::vec3 position = parent->transform->GetLocalPosition() + offset  * parent->transform->GetLocalScale();
        glm::vec3 otherPosition = other->parent->transform->GetLocalPosition() + other->offset  * other->parent->transform->GetLocalScale();
        glm::vec3 difPosition = position - otherPosition;

        glm::vec3 closestVector = glm::vec3(0, 0, 0);
        float closestDistance = glm::distance(otherPosition + glm::normalize(vectors[0]), position);

        for(auto vector : vectors) {
            float distance = glm::distance(otherPosition + glm::normalize(vector), position);
            if (distance < closestDistance) {
                closestDistance = distance;
                closestVector = vector;
                continue;
            }
            if (distance == closestDistance) {
                glm::vec3 point = otherPosition + glm::normalize(vector) * difPosition;
                if(!(point.x >= minOtherPos.x &&
                     point.x <= maxOtherPos.x &&
                     point.y >= minOtherPos.y &&
                     point.y <= maxOtherPos.y &&
                     point.z >= minOtherPos.z &&
                     point.z <= maxOtherPos.z)) {
                    closestDistance = distance;
                    closestVector += vector;
                }
            }
        }

        if (parent->GetComponent<Rigidbody>() != nullptr) {
            glm::vec3 velocity = -parent->GetComponent<Rigidbody>()->velocity;
            velocity = glm::normalize(closestVector) * velocity;
            parent->GetComponent<Rigidbody>()->AddForce(velocity, ForceMode::Impulse);
        }
    }
}

std::vector<glm::vec3> BoxCollider::GetBoxPoints() {
    std::vector<glm::vec3> points;

    points.emplace_back(-1, 1, 1); //0 - LEFT UP FRONT
    points.emplace_back(-1, 1, -1); //1 - LEFT UP BACK
    points.emplace_back(1, 1, 1); //2 - RIGHT UP FRONT
    points.emplace_back(1, 1, -1); //3 - RIGHT UP BACK
    points.emplace_back(-1, -1, 1); //4 - LEFT DOWN FRONT
    points.emplace_back(-1, -1, -1); //5 - LEFT DOWN BACK
    points.emplace_back(1, -1, 1); //6 - RIGHT DOWN FRONT
    points.emplace_back(1, -1, -1); //7 - RIGHT DOWN BACK

    return points;
}

const glm::vec3 &BoxCollider::GetSize() const {
    return size;
}

void BoxCollider::SetSize(const glm::vec3 &size) {
    BoxCollider::size = size;
    Component::OnUpdate();
}

const glm::vec3 &BoxCollider::GetOffset() const {
    return offset;
}

void BoxCollider::SetOffset(const glm::vec3 &offset) {
    BoxCollider::offset = offset;
    Component::OnUpdate();
}

glm::mat4 BoxCollider::GetModelMatrix() {
    return parent->transform->GetModelMatrix() * glm::mat4(size.x, 0, 0, 0, 0, size.y, 0, 0, 0, 0, size.z, 0, offset.x, offset.y, offset.z, 1);
}

bool BoxCollider::GetOBBCollision(std::shared_ptr<BoxCollider> other) {
    const glm::mat4 transformX = glm::rotate(glm::mat4(1.0f), glm::radians(parent->transform->GetLocalRotation().x), glm::vec3(1.0f, 0.0f, 0.0f));
    const glm::mat4 transformY = glm::rotate(glm::mat4(1.0f), glm::radians(parent->transform->GetLocalRotation().y), glm::vec3(0.0f, 1.0f, 0.0f));
    const glm::mat4 transformZ = glm::rotate(glm::mat4(1.0f), glm::radians(parent->transform->GetLocalRotation().z), glm::vec3(0.0f, 0.0f, 1.0f));
    // Y * X * Z
    const glm::mat4 rotationMatrix = transformY * transformX * transformZ;

    const glm::mat4 otherTransformX = glm::rotate(glm::mat4(1.0f), glm::radians(other->parent->transform->GetLocalRotation().x), glm::vec3(1.0f, 0.0f, 0.0f));
    const glm::mat4 otherTransformY = glm::rotate(glm::mat4(1.0f), glm::radians(other->parent->transform->GetLocalRotation().y), glm::vec3(0.0f, 1.0f, 0.0f));
    const glm::mat4 otherTransformZ = glm::rotate(glm::mat4(1.0f), glm::radians(other->parent->transform->GetLocalRotation().z), glm::vec3(0.0f, 0.0f, 1.0f));
    // Y * X * Z
    const glm::mat4 otherRotationMatrix = otherTransformX * otherTransformY * otherTransformZ;

    glm::vec3 otherPos = other->parent->transform->GetLocalPosition();
    otherPos += other->offset * other->parent->transform->GetLocalScale();

    glm::vec3 pos = parent->transform->GetLocalPosition();
    pos += offset * parent->transform->GetLocalScale();

    glm::vec3 t = otherPos - pos;
    t = glm::vec3(glm::dot(t, glm::vec3(rotationMatrix[0])), glm::dot(t, glm::vec3(rotationMatrix[1])), glm::dot(t, glm::vec3(rotationMatrix[2])));

    glm::mat4 rMatrix = otherRotationMatrix * glm::inverse(rotationMatrix);

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            rMatrix[i][j] = fabs(rMatrix[i][j]) + 0.001f;
        }
    }

    glm::vec3 scaledOtherSize = other->size * other->parent->transform->GetLocalScale();

    glm::vec3 scaledSize = size * parent->transform->GetLocalScale();

    // 1
    if (fabs(t.x) > scaledSize.x + (scaledOtherSize.x * fabs(rMatrix[0][0]) + scaledOtherSize.y * fabs(rMatrix[0][1]) +
            scaledOtherSize.z * fabs(rMatrix[0][2]))) return false;
    // 2
    if (fabs(t.y) > scaledSize.y + (scaledOtherSize.x * fabs(rMatrix[1][0]) + scaledOtherSize.y * fabs(rMatrix[1][1]) +
            scaledOtherSize.z * fabs(rMatrix[1][2]))) return false;
    // 3
    if (fabs(t.z) > scaledSize.z + (scaledOtherSize.x * fabs(rMatrix[2][0]) + scaledOtherSize.y * fabs(rMatrix[2][1]) +
            scaledOtherSize.z * fabs(rMatrix[2][2]))) return false;
    // 4
    if (fabs(t.x * rMatrix[0][0] + t.y * rMatrix[1][0] + t.z * rMatrix[2][0]) > (scaledSize.x * fabs(rMatrix[0][0]) + scaledSize.y * fabs(rMatrix[1][0]) +
            scaledSize.z * fabs(rMatrix[2][0])) + scaledOtherSize.x) return false;
    // 5
    if (fabs(t.x * rMatrix[0][1] + t.y * rMatrix[1][1] + t.z * rMatrix[2][1]) > (scaledSize.x * fabs(rMatrix[0][1]) + scaledSize.y * fabs(rMatrix[1][1]) +
            scaledSize.z * fabs(rMatrix[2][1])) + scaledOtherSize.x) return false;
    // 6
    if (fabs(t.x * rMatrix[0][2] + t.y * rMatrix[1][2] + t.z * rMatrix[2][2]) > (scaledSize.x * fabs(rMatrix[0][2]) + scaledSize.y * fabs(rMatrix[1][2]) +
            scaledSize.z * fabs(rMatrix[2][2])) + scaledOtherSize.x) return false;
    // 7
    if (fabs(t.z * rMatrix[1][0] - t.y * rMatrix[2][0]) > (scaledSize.y * fabs(rMatrix[2][0]) + scaledSize.z * fabs(rMatrix[1][0]) +
            scaledOtherSize.y * fabs(rMatrix[0][2]) + scaledOtherSize.z * fabs(rMatrix[0][1]))) return false;
    // 8
    if (fabs(t.z * rMatrix[1][1] - t.y * rMatrix[2][1]) > (scaledSize.y * fabs(rMatrix[2][1]) + scaledSize.z * fabs(rMatrix[1][1]) +
            scaledOtherSize.x * fabs(rMatrix[0][2]) + scaledOtherSize.z * fabs(rMatrix[0][0]))) return false;
    // 9
    if (fabs(t.z * rMatrix[1][2] - t.y * rMatrix[2][2]) > (scaledSize.y * fabs(rMatrix[2][2]) + scaledSize.z * fabs(rMatrix[1][2]) +
            scaledOtherSize.x * fabs(rMatrix[0][1]) + scaledOtherSize.y * fabs(rMatrix[0][0]))) return false;
    // 10
    if (fabs(t.x * rMatrix[2][0] - t.z * rMatrix[0][0]) > (scaledSize.x * fabs(rMatrix[2][0]) + scaledSize.z * fabs(rMatrix[0][0]) +
            scaledOtherSize.y * fabs(rMatrix[1][2]) + scaledOtherSize.z * fabs(rMatrix[1][1]))) return false;
    // 11
    if (fabs(t.x * rMatrix[2][1] - t.z * rMatrix[0][1]) > (scaledSize.x * fabs(rMatrix[2][1]) + scaledSize.z * fabs(rMatrix[0][1]) +
            scaledOtherSize.x * fabs(rMatrix[1][2]) + scaledOtherSize.z * fabs(rMatrix[1][0]))) return false;
    // 12
    if (fabs(t.x * rMatrix[2][2] - t.z * rMatrix[0][2]) > (scaledSize.x * fabs(rMatrix[2][2]) + scaledSize.z * fabs(rMatrix[0][2]) +
            scaledOtherSize.x * fabs(rMatrix[1][1]) + scaledOtherSize.y * fabs(rMatrix[1][0]))) return false;
    // 13
    if (fabs(t.y * rMatrix[0][0] - t.x * rMatrix[1][0]) > (scaledSize.x * fabs(rMatrix[1][0]) + scaledSize.y * fabs(rMatrix[0][0]) +
            scaledOtherSize.y * fabs(rMatrix[2][2]) + scaledOtherSize.z * fabs(rMatrix[2][1]))) return false;
    // 14
    if (fabs(t.y * rMatrix[0][1] - t.x * rMatrix[1][1]) > (scaledSize.x * fabs(rMatrix[1][1]) + scaledSize.y * fabs(rMatrix[0][1]) +
            scaledOtherSize.x * fabs(rMatrix[2][2]) + scaledOtherSize.z * fabs(rMatrix[2][0]))) return false;
    // 15
    if (fabs(t.y * rMatrix[0][2] - t.x * rMatrix[1][2]) > (scaledSize.x * fabs(rMatrix[1][1]) + scaledSize.y * fabs(rMatrix[0][2]) +
            scaledOtherSize.x * fabs(rMatrix[2][1]) + scaledOtherSize.y * fabs(rMatrix[2][0]))) return false;
    return true;
}

