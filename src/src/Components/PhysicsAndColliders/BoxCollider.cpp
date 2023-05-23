#include "Components/PhysicsAndColliders/BoxCollider.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "EngineManagers/CollisionManager.h"
#include "Components/PhysicsAndColliders/Rigidbody.h"
#include "EngineManagers/AIManager.h"

#include <cmath>

#ifdef DEBUG
#include <tracy/Tracy.hpp>
#endif

BoxCollider::BoxCollider(const std::shared_ptr<GameObject> &parent, int id)
        : Component(parent, id) {
    size = {1.0f, 1.0f, 1.0f};
    offset = {0.0f, 0.0f, 0.0f};
}

BoxCollider::~BoxCollider() = default;

void BoxCollider::Start() {
    SetGridPoints();
    Component::Start();
}

void BoxCollider::FixedUpdate() {
//#ifdef DEBUG
//    ZoneScopedNC("BoxCollider", 0xf0fc03);
//#endif

    if (!parent->GetComponent<Rigidbody>())
        return;

    CollisionManager::GetInstance()->RemoveDynamicBoxCollider(parent->transform->GetGlobalPosition(), id);
    SetGridPoints();
    Component::FixedUpdate();
}


void BoxCollider::OnDestroy() {
    CollisionManager::GetInstance()->RemoveBoxCollider(id);
//    AIManager::GetInstance()->RemoveBoxCollider(std::dynamic_pointer_cast<BoxCollider>(shared_from_this()));
#ifdef DEBUG
    CollisionManager::GetInstance()->OnBoxCollidersChange();
#endif
    Component::OnDestroy();
}

void BoxCollider::CheckCollision(const std::shared_ptr<BoxCollider>& other) {
//#ifdef DEBUG
//    ZoneScopedNC("CheckCollision", 0x03adfc);
//#endif

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

        if (other->isTrigger) return;

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
#ifdef DEBUG
    ZoneScopedNC("GetOBBCollision", 0x0339fc);
#endif

    const glm::mat4 transformX = glm::rotate(glm::mat4(1.0f), glm::radians(parent->globalRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    const glm::mat4 transformY = glm::rotate(glm::mat4(1.0f), glm::radians(parent->globalRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    const glm::mat4 transformZ = glm::rotate(glm::mat4(1.0f), glm::radians(parent->globalRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    // Y * X * Z
    const glm::mat4 rotationMatrix = transformY * transformX * transformZ;

    const glm::mat4 otherTransformX = glm::rotate(glm::mat4(1.0f), glm::radians(other->parent->globalRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    const glm::mat4 otherTransformY = glm::rotate(glm::mat4(1.0f), glm::radians(other->parent->globalRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    const glm::mat4 otherTransformZ = glm::rotate(glm::mat4(1.0f), glm::radians(other->parent->globalRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
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

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            rMatrix[i][j] = glm::dot(vectors[i], otherVectors[j]);
        }
    }

    glm::mat3 absRMatrix;

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            absRMatrix[i][j] = fabs(rMatrix[i][j]) + 0.001f;
        }
    }

    glm::vec3 scaledOtherSize = other->size * other->parent->transform->GetGlobalScale();
    glm::vec3 scaledSize = size * parent->transform->GetGlobalScale();

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

    auto points = GetBoxPoints();

    auto boxPoints = points;
    auto otherModelMatrix = other->GetModelMatrix();

    for (int i = 0; i < points.size(); ++i) {
        boxPoints[i] = glm::vec3(otherModelMatrix * glm::vec4(boxPoints[i], 1));
    }

    float minDistance = glm::distance(boxPoints[0], position);
    auto closestPoint = points[0];
    auto closestBoxPoint = boxPoints[0];

    for (int i = 0; i < boxPoints.size(); ++i) {
        float distance = glm::distance(boxPoints[i], position);
        if (distance < minDistance) {
            minDistance = distance;
            closestPoint = points[i];
            closestBoxPoint = boxPoints[i];
        }
    }

    const glm::mat4 transformX = glm::rotate(glm::mat4(1.0f), glm::radians(other->parent->globalRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    const glm::mat4 transformY = glm::rotate(glm::mat4(1.0f), glm::radians(other->parent->globalRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    const glm::mat4 transformZ = glm::rotate(glm::mat4(1.0f), glm::radians(other->parent->globalRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

    // Y * X * Z
    const glm::mat4 rotationMatrix = transformY * transformX * transformZ;

    glm::vec3 normals[3];
    normals[0] = rotationMatrix * glm::vec4(closestPoint.x, 0, 0, 1);
    normals[1] = rotationMatrix * glm::vec4(0, closestPoint.y, 0, 1);
    normals[2] = rotationMatrix * glm::vec4(0, 0, closestPoint.z, 1);

    glm::vec3 newPoints[3];
    newPoints[0] = closestBoxPoint + normals[0];
    newPoints[1] = closestBoxPoint + normals[1];
    newPoints[2] = closestBoxPoint + normals[2];

    minDistance = glm::distance(newPoints[0], position);
    auto closestVector = normals[0];

    for (int i = 0; i < 3; ++i) {
        float distance = glm::distance(newPoints[i], position);
        if (distance < minDistance) {
            minDistance = distance;
            closestVector = normals[i];
        }
    }

    closestVector = glm::normalize(closestVector);

    if (parent->GetComponent<Rigidbody>() != nullptr) {
        glm::vec3 otherRotation = other->parent->globalRotation;

        // Collision handling for not rotated collider
        if ((int)std::round(otherRotation.x) % 90 == 0 && (int)std::round(otherRotation.y) % 90 == 0 &&
        (int)std::round(otherRotation.z) % 90 == 0) {
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


void BoxCollider::SetGridPoints() {
#ifdef DEBUG
    ZoneScopedNC("SGP", 0x0339fc);
#endif
    glm::mat4 model = GetModelMatrix();

    const glm::mat4 transformX = glm::rotate(glm::mat4(1.0f), glm::radians(parent->globalRotation.x),
                                             glm::vec3(1.0f, 0.0f, 0.0f));
    const glm::mat4 transformY = glm::rotate(glm::mat4(1.0f), glm::radians(parent->globalRotation.y),
                                             glm::vec3(0.0f, 1.0f, 0.0f));
    const glm::mat4 transformZ = glm::rotate(glm::mat4(1.0f), glm::radians(parent->globalRotation.z),
                                             glm::vec3(0.0f, 0.0f, 1.0f));

    // Y * X * Z
    const glm::mat4 rotationMatrix = transformY * transformX * transformZ;

    glm::vec4 pos = model * glm::vec4(0, 0, 0, 1);

    glm::vec4 xVec = (rotationMatrix * glm::vec4(1, 0, 0, 1)) * size.x * parent->transform->GetGlobalScale().x;
    glm::vec4 zVec = (rotationMatrix * glm::vec4(0, 0, 1, 1)) * size.z * parent->transform->GetGlobalScale().z;

    auto xVector = glm::vec2(xVec.x, xVec.z);
    auto zVector = glm::vec2(zVec.x, zVec.z);

    const float colGridSize = CollisionManager::GetInstance()->gridSize;

    glm::ivec2 colGridPoints[4] = {
            glm::ivec2((glm::vec2(pos.x, pos.z) + (xVector + zVector)) / colGridSize),
            glm::ivec2((glm::vec2(pos.x, pos.z) + (xVector - zVector)) / colGridSize),
            glm::ivec2((glm::vec2(pos.x, pos.z) + (-xVector + zVector)) / colGridSize),
            glm::ivec2((glm::vec2(pos.x, pos.z) + (-xVector - zVector)) / colGridSize)
    };

    SetCollidersGridPoints(colGridPoints);

    if (!isTrigger && !parent->GetComponent<Rigidbody>() && std::strcmp(parent->GetName().c_str(), "Ground") != 0) {
        const float aiCellSize = AIManager::GetInstance()->aiCellSize;

        glm::ivec2 aiGridPoints[4] = {
                glm::ivec2((glm::vec2(pos.x, pos.z) + (xVector + zVector)) / aiCellSize),
                glm::ivec2((glm::vec2(pos.x, pos.z) + (xVector - zVector)) / aiCellSize),
                glm::ivec2((glm::vec2(pos.x, pos.z) + (-xVector + zVector)) / aiCellSize),
                glm::ivec2((glm::vec2(pos.x, pos.z) + (-xVector - zVector)) / aiCellSize)
        };

        SetAIGridPoints(aiGridPoints);
    }
}

void BoxCollider::SetCollidersGridPoints(const glm::ivec2* points) {
#ifdef DEBUG
    ZoneScopedNC("SCGP", 0x0339fc);
#endif

    if (points[0] == points[1] &&
        points[1] == points[2] &&
        points[2] == points[3]) {
        int x = points[0].x;
        int y = points[0].y;

        CollisionManager::GetInstance()->grid[(x + GRID_SIZE / 2) + (y + GRID_SIZE / 2) * GRID_SIZE].insert(
                {id, std::dynamic_pointer_cast<BoxCollider>(shared_from_this())});
        return;
    }

    int minX = points[0].x;
    int minY = points[0].y;
    int maxX = points[0].x;
    int maxY = points[0].y;

    for (int i = 1; i < 4; i++) {
        if (minX > points[i].x) minX = points[i].x;
        if (minY > points[i].y) minY = points[i].y;
        if (maxX < points[i].x) maxX = points[i].x;
        if (maxY < points[i].y) maxY = points[i].y;
    }

    for (int x = minX; x <= maxX; ++x) {
        for (int y = minY; y <= maxY; ++y) {
            CollisionManager::GetInstance()->grid[(x + GRID_SIZE / 2) + (y + GRID_SIZE / 2) * GRID_SIZE].insert(
                    {id, std::dynamic_pointer_cast<BoxCollider>(shared_from_this())});
        }
    }
}

void BoxCollider::SetAIGridPoints(const glm::ivec2* points) {
#ifdef DEBUG
    ZoneScopedNC("SAIGP", 0x0339fc);
#endif

    if (points[0] == points[1] &&
        points[1] == points[2] &&
        points[2] == points[3]) {
        int x = points[0].x;
        int y = points[0].y;

        AIManager::GetInstance()->aiGrid[x + AI_GRID_SIZE / 2][y + AI_GRID_SIZE / 2] = true;
        return;
    }

    int minX = points[0].x;
    int minY = points[0].y;
    int maxX = points[0].x;
    int maxY = points[0].y;

    for (int i = 1; i < 4; i++) {
        if (minX > points[i].x) minX = points[i].x;
        if (minY > points[i].y) minY = points[i].y;
        if (maxX < points[i].x) maxX = points[i].x;
        if (maxY < points[i].y) maxY = points[i].y;
    }

    for (int x = minX - 1; x <= maxX + 1; ++x) {
        for (int y = minY - 1; y <= maxY + 1; ++y) {
            AIManager::GetInstance()->aiGrid[x + AI_GRID_SIZE / 2][y + AI_GRID_SIZE / 2] = true;
        }
    }
}
