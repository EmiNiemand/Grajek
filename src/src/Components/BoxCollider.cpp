#include "include/Components/BoxCollider.h"
#include "include/GloomEngine.h"
#include "include/HighLevelClasses/GameObject.h"
#include "include/EngineComponents/EngineColliders.h"

BoxCollider::BoxCollider(const std::shared_ptr<GloomEngine> &gloomEngine, const std::shared_ptr<GameObject> &parent)
        : Component(gloomEngine, parent) {
    name = ComponentNames::BOXCOLLIDER;
    size = {1.0f, 1.0f, 1.0f};
    offset = {0.0f, 0.0f, 0.0f};
    AxisX = {1, 0, 0};
    AxisY = {0, 1, 0};
    AxisZ = {0, 0, 1};
}

BoxCollider::~BoxCollider() {}

void BoxCollider::HandleCollision(std::shared_ptr<BoxCollider> other) {
    glm::vec3 minBoxPos = GetModelMatrix() * glm::vec4(-1, -1, -1, 1);
    glm::vec3 maxBoxPos = GetModelMatrix() * glm::vec4(1, 1, 1, 1);

    glm::vec3 minOtherPos = other->GetModelMatrix() * glm::vec4(-1, -1, -1, 1);
    glm::vec3 maxOtherPos = other->GetModelMatrix() * glm::vec4(1, 1, 1, 1);

    // Collision checker for not rotated cubes
    if (parent->transform->GetLocalRotation() == glm::vec3(0, 0, 0) && other->parent->transform->GetLocalRotation() == glm::vec3(0, 0, 0)){
        if (minBoxPos.x <= maxOtherPos.x && maxBoxPos.x >= minOtherPos.x && minBoxPos.y <= maxOtherPos.y &&
            maxBoxPos.y >= minOtherPos.y && minBoxPos.z <= maxOtherPos.z && maxBoxPos.z >= minOtherPos.z){
            spdlog::info("HIT");
           // TODO: Separate depending on velocity of objects - physics depended
            glm::vec3 pos1 = parent->transform->GetGlobalPosition();
            glm::vec3 pos2 = other->parent->transform->GetGlobalPosition();
            glm::vec3 vector = pos2 - pos1;

            vector /= 2;
            spdlog::info(std::to_string(vector.x) + ", " + std::to_string(vector.y) + ", " + std::to_string(vector.z));

            other->parent->transform->SetLocalPosition(other->parent->transform->GetLocalPosition() + vector);
            return;
        }
    }

    if (GetOBBCollision(other)) {
       // TODO: Separate depending on velocity of objects - physics depended
        glm::vec3 pos1 = parent->transform->GetGlobalPosition();
        glm::vec3 pos2 = other->parent->transform->GetGlobalPosition();
        glm::vec3 vector = pos2 - pos1;

        vector /= 2;
        spdlog::info(std::to_string(vector.x) + ", " + std::to_string(vector.y) + ", " + std::to_string(vector.z));

        other->parent->transform->SetLocalPosition(other->parent->transform->GetLocalPosition() + vector);
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

void BoxCollider::OnRemove() {
    gloomEngine->engineColliders->RemoveBoxCollider(shared_from_this());
    Component::OnRemove();
}

const glm::vec3 &BoxCollider::GetSize() const {
    return size;
}

void BoxCollider::SetSize(const glm::vec3 &size) {
    BoxCollider::size = size;
    gloomEngine->engineColliders->OnBoxColliderChange();
}

const glm::vec3 &BoxCollider::GetOffset() const {
    return offset;
}

void BoxCollider::SetOffset(const glm::vec3 &offset) {
    BoxCollider::offset = offset;
    gloomEngine->engineColliders->OnBoxColliderChange();
}

glm::mat4 BoxCollider::GetModelMatrix() {
    return parent->transform->GetModelMatrix() * glm::mat4(size.x, 0, 0, 0, 0, size.y, 0, 0, 0, 0, size.z, 0, offset.x, offset.y, offset.z, 1);
}

void BoxCollider::OnTransformUpdate() {
    AxisX = parent->transform->GetLocalRotation().x * glm::vec4(AxisX, 1);
    AxisY = parent->transform->GetLocalRotation().y * glm::vec4(AxisY, 1);
    AxisZ = parent->transform->GetLocalRotation().z * glm::vec4(AxisZ, 1);
    Component::OnTransformUpdate();
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

    glm::vec3 otherPos = other->parent->transform->GetGlobalPosition();
    otherPos += other->offset;

    glm::vec3 pos = parent->transform->GetGlobalPosition();
    pos += offset;

    glm::vec3 t = otherPos - pos;
    t = glm::vec3(glm::dot(t, glm::vec3(rotationMatrix[0])), glm::dot(t, glm::vec3(rotationMatrix[1])), glm::dot(t, glm::vec3(rotationMatrix[2])));

    glm::mat4 rMatrix = otherRotationMatrix * glm::inverse(rotationMatrix);

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            rMatrix[i][j] = fabs(rMatrix[i][j]) + 0.001f;
        }
    }

    glm::vec3 otherSize = other->size;

    // 1
    if (fabs(t.x) > size.x + (otherSize.x * fabs(rMatrix[0][0]) + otherSize.y * fabs(rMatrix[0][1]) +
            otherSize.z * fabs(rMatrix[0][2]))) return false;
    // 2
    if (fabs(t.y) > size.y + (otherSize.x * fabs(rMatrix[1][0]) + otherSize.y * fabs(rMatrix[1][1]) +
            otherSize.z * fabs(rMatrix[1][2]))) return false;
    // 3
    if (fabs(t.z) > size.z + (otherSize.x * fabs(rMatrix[2][0]) + otherSize.y * fabs(rMatrix[2][1]) +
            otherSize.z * fabs(rMatrix[2][2]))) return false;
    // 4
    if (fabs(t.x * rMatrix[0][0] + t.y * rMatrix[1][0] + t.z * rMatrix[2][0]) > (size.x * fabs(rMatrix[0][0]) + size.y * fabs(rMatrix[1][0]) +
                                                           size.z * fabs(rMatrix[2][0])) + otherSize.x) return false;
    // 5
    if (fabs(t.x * rMatrix[0][1] + t.y * rMatrix[1][1] + t.z * rMatrix[2][1]) > (size.x * fabs(rMatrix[0][1]) + size.y * fabs(rMatrix[1][1]) +
                                                           size.z * fabs(rMatrix[2][1])) + otherSize.x) return false;
    // 6
    if (fabs(t.x * rMatrix[0][2] + t.y * rMatrix[1][2] + t.z * rMatrix[2][2]) > (size.x * fabs(rMatrix[0][2]) + size.y * fabs(rMatrix[1][2]) +
                                                           size.z * fabs(rMatrix[2][2])) + otherSize.x) return false;
    // 7
    if (fabs(t.z * rMatrix[1][0] - t.y * rMatrix[2][0]) > (size.y * fabs(rMatrix[2][0]) + size.z * fabs(rMatrix[1][0]) +
            otherSize.y * fabs(rMatrix[0][2]) + otherSize.z * fabs(rMatrix[0][1]))) return false;
    // 8
    if (fabs(t.z * rMatrix[1][1] - t.y * rMatrix[2][1]) > (size.y * fabs(rMatrix[2][1]) + size.z * fabs(rMatrix[1][1]) +
            otherSize.x * fabs(rMatrix[0][2]) + otherSize.z * fabs(rMatrix[0][0]))) return false;
    // 9
    if (fabs(t.z * rMatrix[1][2] - t.y * rMatrix[2][2]) > (size.y * fabs(rMatrix[2][2]) + size.z * fabs(rMatrix[1][2]) +
            otherSize.x * fabs(rMatrix[0][1]) + otherSize.y * fabs(rMatrix[0][0]))) return false;
    // 10
    if (fabs(t.x * rMatrix[2][0] - t.z * rMatrix[0][0]) > (size.x * fabs(rMatrix[2][0]) + size.z * fabs(rMatrix[0][0]) +
            otherSize.y * fabs(rMatrix[1][2]) + otherSize.z * fabs(rMatrix[1][1]))) return false;
    // 11
    if (fabs(t.x * rMatrix[2][1] - t.z * rMatrix[0][1]) > (size.x * fabs(rMatrix[2][1]) + size.z * fabs(rMatrix[0][1]) +
            otherSize.x * fabs(rMatrix[1][2]) + otherSize.z * fabs(rMatrix[1][0]))) return false;
    // 12
    if (fabs(t.x * rMatrix[2][2] - t.z * rMatrix[0][2]) > (size.x * fabs(rMatrix[2][2]) + size.z * fabs(rMatrix[0][2]) +
            otherSize.x * fabs(rMatrix[1][1]) + otherSize.y * fabs(rMatrix[1][0]))) return false;
    // 13
    if (fabs(t.y * rMatrix[0][0] - t.x * rMatrix[1][0]) > (size.x * fabs(rMatrix[1][0]) + size.y * fabs(rMatrix[0][0]) +
            otherSize.y * fabs(rMatrix[2][2]) + otherSize.z * fabs(rMatrix[2][1]))) return false;
    // 14
    if (fabs(t.y * rMatrix[0][1] - t.x * rMatrix[1][1]) > (size.x * fabs(rMatrix[1][1]) + size.y * fabs(rMatrix[0][1]) +
            otherSize.x * fabs(rMatrix[2][2]) + otherSize.z * fabs(rMatrix[2][0]))) return false;
    // 15
    if (fabs(t.y * rMatrix[0][2] - t.x * rMatrix[1][2]) > (size.x * fabs(rMatrix[1][1]) + size.y * fabs(rMatrix[0][2]) +
            otherSize.x * fabs(rMatrix[2][1]) + otherSize.y * fabs(rMatrix[2][0]))) return false;
    return true;
}

