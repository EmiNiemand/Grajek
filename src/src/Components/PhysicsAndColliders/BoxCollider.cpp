#include "Components/PhysicsAndColliders/BoxCollider.h"
#include "GloomEngine.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "EngineManagers/ColliderManager.h"
#include "Components/PhysicsAndColliders/Rigidbody.h"

BoxCollider::BoxCollider(const std::shared_ptr<GameObject> &parent, int id)
        : Component(parent, id) {
    size = {1.0f, 1.0f, 1.0f};
    offset = {0.0f, 0.0f, 0.0f};
}

BoxCollider::~BoxCollider() = default;

void BoxCollider::OnCreate() {
    ColliderManager::GetInstance()->boxColliders.insert({id, std::dynamic_pointer_cast<BoxCollider>(shared_from_this())});
    ColliderManager::GetInstance()->OnBoxCollidersChange();
    Component::OnCreate();
}

void BoxCollider::OnDestroy() {
    ColliderManager::GetInstance()->RemoveBoxCollider(id);
    ColliderManager::GetInstance()->OnBoxCollidersChange();
    Component::OnDestroy();
}

void BoxCollider::CheckCollision(const std::shared_ptr<BoxCollider>& other) {
    bool isColliding = GetOBBCollision(other);

    if (!isColliding) {
        // OnTriggerExit
        if (isTrigger && collisionsBuffer.contains(other->id)) {
            for (const auto& component : parent->components) component.second->OnTriggerExit(other->parent);
            collisionsBuffer.erase(other->id);
            return;
        }
    }

    // OBB
    if (isColliding) {
        // OnTriggerEnter
        if (isTrigger && !collisionsBuffer.contains(other->id)) {
            for (const auto& component : parent->components) component.second->OnTriggerEnter(other->parent);
            collisionsBuffer.insert({other->id, other->parent});
        }
        // OnTriggerStay
        if (isTrigger && collisionsBuffer.contains(other->id)) {
            for (const auto& component : parent->components) component.second->OnTriggerStay(other->parent);
            return;
        }

        HandleCollision(other);
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

bool BoxCollider::GetOBBCollision(const std::shared_ptr<BoxCollider>& other) {
    const glm::mat4 transformX = glm::rotate(glm::mat4(1.0f), glm::radians(parent->transform->GetLocalRotation().x), glm::vec3(1.0f, 0.0f, 0.0f));
    const glm::mat4 transformY = glm::rotate(glm::mat4(1.0f), glm::radians(parent->transform->GetLocalRotation().y), glm::vec3(0.0f, 1.0f, 0.0f));
    const glm::mat4 transformZ = glm::rotate(glm::mat4(1.0f), glm::radians(parent->transform->GetLocalRotation().z), glm::vec3(0.0f, 0.0f, 1.0f));
    // Y * X * Z
    const glm::mat4 rotationMatrix = transformY * transformX * transformZ;

    const glm::mat4 otherTransformX = glm::rotate(glm::mat4(1.0f), glm::radians(other->parent->transform->GetLocalRotation().x), glm::vec3(1.0f, 0.0f, 0.0f));
    const glm::mat4 otherTransformY = glm::rotate(glm::mat4(1.0f), glm::radians(other->parent->transform->GetLocalRotation().y), glm::vec3(0.0f, 1.0f, 0.0f));
    const glm::mat4 otherTransformZ = glm::rotate(glm::mat4(1.0f), glm::radians(other->parent->transform->GetLocalRotation().z), glm::vec3(0.0f, 0.0f, 1.0f));
    // Y * X * Z
    const glm::mat4 otherRotationMatrix = otherTransformY * otherTransformX * otherTransformZ;

    glm::vec3 otherPos = other->GetModelMatrix() * glm::vec4(0,0,0,1);
    glm::vec3 pos = GetModelMatrix() * glm::vec4(0,0,0,1);

    glm::vec3 vectors[3];
    vectors[0] = glm::vec3(rotationMatrix * glm::vec4(1,0,0,1));
    vectors[1] = glm::vec3(rotationMatrix * glm::vec4(0,1,0,1));
    vectors[2] = glm::vec3(rotationMatrix * glm::vec4(0,0,1,1));

    glm::vec3 otherVectors[3];
    otherVectors[0] = glm::vec3(otherRotationMatrix * glm::vec4(1,0,0,1));
    otherVectors[1] = glm::vec3(otherRotationMatrix * glm::vec4(0,1,0,1));
    otherVectors[2] = glm::vec3(otherRotationMatrix * glm::vec4(0,0,1,1));

    glm::vec3 t = otherPos - pos;
    t = glm::vec3(glm::dot(t, vectors[0]), glm::dot(t, vectors[1]), glm::dot(t, vectors[2]));

    auto rMatrix = glm::mat3(1.0f);

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            rMatrix[i][j] = glm::dot(vectors[i], otherVectors[j]);
        }
    }

    glm::mat3 absRMatrix;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            absRMatrix[i][j] = fabs(rMatrix[i][j]) + 0.001f;
        }
    }

    glm::vec3 scaledOtherSize = other->size * other->parent->transform->GetLocalScale();
    glm::vec3 scaledSize = size * parent->transform->GetLocalScale();

    // 1
    if (fabs(t.x) > scaledSize.x + (scaledOtherSize.x * absRMatrix[0][0] + scaledOtherSize.y * absRMatrix[0][1] +
            scaledOtherSize.z * absRMatrix[0][2])) return false;
    // 2
    if (fabs(t.y) > scaledSize.y + (scaledOtherSize.x * absRMatrix[1][0] + scaledOtherSize.y * absRMatrix[1][1] +
            scaledOtherSize.z * absRMatrix[1][2])) return false;
    // 3
    if (fabs(t.z) > scaledSize.z + (scaledOtherSize.x * absRMatrix[2][0] + scaledOtherSize.y * absRMatrix[2][1] +
            scaledOtherSize.z * absRMatrix[2][2])) return false;
    // 4
    if (fabs(t.x * rMatrix[0][0] + t.y * rMatrix[1][0] + t.z * rMatrix[2][0]) > (scaledSize.x * absRMatrix[0][0] + scaledSize.y * absRMatrix[1][0] +
            scaledSize.z * absRMatrix[2][0]) + scaledOtherSize.x) return false;
    // 5
    if (fabs(t.x * rMatrix[0][1] + t.y * rMatrix[1][1] + t.z * rMatrix[2][1]) > (scaledSize.x * absRMatrix[0][1] + scaledSize.y * absRMatrix[1][1] +
            scaledSize.z * absRMatrix[2][1]) + scaledOtherSize.y) return false;
    // 6
    if (fabs(t.x * rMatrix[0][2] + t.y * rMatrix[1][2] + t.z * rMatrix[2][2]) > (scaledSize.x * absRMatrix[0][2] + scaledSize.y * absRMatrix[1][2] +
            scaledSize.z * absRMatrix[2][2]) + scaledOtherSize.z) return false;
    // 7
    if (fabs(t.z * rMatrix[1][0] - t.y * rMatrix[2][0]) > (scaledSize.y * absRMatrix[2][0] + scaledSize.z * absRMatrix[1][0] +
            scaledOtherSize.y * absRMatrix[0][2] + scaledOtherSize.z * absRMatrix[0][1])) return false;
    // 8
    if (fabs(t.z * rMatrix[1][1] - t.y * rMatrix[2][1]) > (scaledSize.y * absRMatrix[2][1] + scaledSize.z * absRMatrix[1][1] +
            scaledOtherSize.x * absRMatrix[0][2] + scaledOtherSize.z * absRMatrix[0][0])) return false;
    // 9
    if (fabs(t.z * rMatrix[1][2] - t.y * rMatrix[2][2]) > (scaledSize.y * absRMatrix[2][2] + scaledSize.z * absRMatrix[1][2] +
            scaledOtherSize.x * absRMatrix[0][1] + scaledOtherSize.y * absRMatrix[0][0])) return false;
    // 10
    if (fabs(t.x * rMatrix[2][0] - t.z * rMatrix[0][0]) > (scaledSize.x * absRMatrix[2][0] + scaledSize.z * absRMatrix[0][0] +
            scaledOtherSize.y * absRMatrix[1][2] + scaledOtherSize.z * absRMatrix[1][1])) return false;
    // 11
    if (fabs(t.x * rMatrix[2][1] - t.z * rMatrix[0][1]) > (scaledSize.x * absRMatrix[2][1] + scaledSize.z * absRMatrix[0][1] +
            scaledOtherSize.x * absRMatrix[1][2] + scaledOtherSize.z * absRMatrix[1][0])) return false;
    // 12
    if (fabs(t.x * rMatrix[2][2] - t.z * rMatrix[0][2]) > (scaledSize.x * absRMatrix[2][2] + scaledSize.z * absRMatrix[0][2] +
            scaledOtherSize.x * absRMatrix[1][1] + scaledOtherSize.y * absRMatrix[1][0])) return false;
    // 13
    if (fabs(t.y * rMatrix[0][0] - t.x * rMatrix[1][0]) > (scaledSize.x * absRMatrix[1][0] + scaledSize.y * absRMatrix[0][0] +
            scaledOtherSize.y * absRMatrix[2][2] + scaledOtherSize.z * absRMatrix[2][1])) return false;
    // 14
    if (fabs(t.y * rMatrix[0][1] - t.x * rMatrix[1][1]) > (scaledSize.x * absRMatrix[1][1] + scaledSize.y * absRMatrix[0][1] +
            scaledOtherSize.x * absRMatrix[2][2] + scaledOtherSize.z * absRMatrix[2][0])) return false;
    // 15
    if (fabs(t.y * rMatrix[0][2] - t.x * rMatrix[1][2]) > (scaledSize.x * absRMatrix[1][2] + scaledSize.y * absRMatrix[0][2] +
            scaledOtherSize.x * absRMatrix[2][1] + scaledOtherSize.y * absRMatrix[2][0])) return false;

    return true;
}

void BoxCollider::HandleCollision(const std::shared_ptr<BoxCollider> &other) {
    glm::vec3 otherPosition = other->GetModelMatrix() * glm::vec4(0,0,0,1);
    glm::vec3 position = GetModelMatrix() * glm::vec4(0,0,0,1);

    std::vector<std::pair<glm::vec3, glm::vec3>> points = CalculateShiftedPoints(other, position, otherPosition);

    if (points.empty()) return;
    glm::vec3 closestVector = GetClosestShiftedPoint(points, position);
    closestVector = glm::normalize(closestVector);

    if (parent->GetComponent<Rigidbody>() != nullptr) {
        float cos = glm::dot(glm::vec3(1, 0, 0), closestVector);
        float absCos = std::abs(cos);
        // Collision handling for not rotated collider
        if (absCos >= -0.0001 && absCos <= 0.0001 || absCos >= 1 - 0.0001 && absCos <= 1 + 0.0001) {
            float value = closestVector.x + closestVector.y + closestVector.z;
            glm::vec3 velocityOffset = closestVector * glm::vec3(0.001);

            glm::vec3 velocity = closestVector * parent->GetComponent<Rigidbody>()->velocity;
            if (std::round(closestVector.y) == 1) {
                velocityOffset = glm::vec3(0);
            }
            if (value > 0) {
                velocity = -velocity;
            }
            parent->GetComponent<Rigidbody>()->AddForce(velocity + velocityOffset, ForceMode::Impulse);
        }
            // Collision handling for rotated colliders
        else {
            glm::vec3 velocity = parent->GetComponent<Rigidbody>()->velocity;
            float velocityLength = glm::length(velocity);
            glm::vec3 vel = closestVector * 0.001f;
            if(velocityLength < 0.01f) {
                vel = closestVector * 0.01f;
            }

            parent->GetComponent<Rigidbody>()->velocity = glm::vec3(0);
            parent->GetComponent<Rigidbody>()->AddForce(vel, ForceMode::Impulse);
        }
    }
}

std::vector<std::pair<glm::vec3, glm::vec3>>
BoxCollider::CalculateShiftedPoints(const std::shared_ptr<BoxCollider> &other, glm::vec3 position, glm::vec3 otherPosition) {
    glm::vec3 rotation = parent->transform->GetLocalRotation();
    glm::vec3 otherRotation = other->parent->transform->GetLocalRotation();

    glm::vec3 minBoxPos = GetModelMatrix() * glm::vec4(-1, -1, -1, 1);
    glm::vec3 maxBoxPos = GetModelMatrix() * glm::vec4(1, 1, 1, 1);

    glm::vec3 minOtherPos = other->GetModelMatrix() * glm::vec4(-1, -1, -1, 1);
    glm::vec3 maxOtherPos = other->GetModelMatrix() * glm::vec4(1, 1, 1, 1);

    const glm::mat4 transformX = glm::rotate(glm::mat4(1.0f), glm::radians(otherRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    const glm::mat4 transformY = glm::rotate(glm::mat4(1.0f), glm::radians(otherRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    const glm::mat4 transformZ = glm::rotate(glm::mat4(1.0f), glm::radians(otherRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

    // Y * X * Z
    const glm::mat4 rotationMatrix = transformY * transformX * transformZ;

    glm::vec3 diffPos = position - otherPosition;
    diffPos = glm::vec3(std::abs(diffPos.x), std::abs(diffPos.y), std::abs(diffPos.z));

    // Walls' normal vectors
    glm::vec3 vectors[6];
    vectors[0] = glm::vec3(rotationMatrix * glm::vec4(1,0,0,1));
    vectors[1] = glm::vec3(rotationMatrix * glm::vec4(0,1,0,1));
    vectors[2] = glm::vec3(rotationMatrix * glm::vec4(0,0,-1,1));
    vectors[3] = -vectors[0];
    vectors[4] = -vectors[1];
    vectors[5] = -vectors[2];

    std::vector<std::pair<glm::vec3, glm::vec3>> points;

    // Calculate shifted point in normal direction to check if it is within the walls of other collider and to look
    // the closest point to the first collider
    for (auto vector : vectors) {
        glm::vec3 point = otherPosition + glm::normalize(vector) * diffPos;
        if(!(point.x >= minOtherPos.x && point.x <= maxOtherPos.x &&
             point.y >= minOtherPos.y && point.y <= maxOtherPos.y &&
             point.z >= minOtherPos.z && point.z <= maxOtherPos.z)) {
            points.emplace_back(point, vector);
        }
    }

    return points;
}

glm::vec3 BoxCollider::GetClosestShiftedPoint(std::vector<std::pair<glm::vec3, glm::vec3>> points, glm::vec3 position) {
    float minDistance = glm::distance(points[0].first, position);
    glm::vec3 closestVector = glm::vec3(0, 0, 0);

    for (auto point : points) {
        float distance = glm::distance(point.first, position);
        if (distance < minDistance + 0.0001 && distance > minDistance - 0.0001) {
            closestVector += point.second;
        }
        else if (distance < minDistance - 0.0001) {
            minDistance = distance;
            closestVector = point.second;
        }
    }
    return closestVector;
}

