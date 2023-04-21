#ifndef GLOOMENGINE_FRUSTUMCULLING_H
#define GLOOMENGINE_FRUSTUMCULLING_H

#include "glm/matrix.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <memory>
#include <vector>
#include "Components/Transform.h"

class Model;

struct Plane
{
    glm::vec3 normal = { 0.f, 1.f, 0.f }; // unit vector
    float distance = 0.f; // Distance with origin

    Plane() = default;

    Plane(const glm::vec3& p1, const glm::vec3& norm) : normal(glm::normalize(norm)), distance(glm::dot(normal, p1)){}

    [[nodiscard]] float GetSignedDistanceToPlane(const glm::vec3& point) const
    {
        return glm::dot(normal, point) - distance;
    }
};

struct Frustum
{
    Plane topFace;
    Plane bottomFace;

    Plane rightFace;
    Plane leftFace;

    Plane farFace;
    Plane nearFace;
};

struct AABB
{
    glm::vec3 center{ 0.f, 0.f, 0.f };
    glm::vec3 extents{ 0.f, 0.f, 0.f };

    AABB(const glm::vec3& min, const glm::vec3& max) : center{ (max + min) * 0.5f },
    extents{ max.x - center.x, max.y - center.y, max.z - center.z } {}

    AABB(const glm::vec3& inCenter, float iI, float iJ, float iK) : center{ inCenter }, extents{ iI, iJ, iK } {}

    //see https://gdbooks.gitbooks.io/3dcollisions/content/Chapter2/static_aabb_plane.html
    [[nodiscard]] bool IsOnOrForwardPlane(const Plane& plane) const
    {
        // Compute the projection interval radius of b onto L(t) = b.c + t * p.n
        const float r = extents.x * std::abs(plane.normal.x) + extents.y * std::abs(plane.normal.y) +
                        extents.z * std::abs(plane.normal.z);

        return -r <= plane.GetSignedDistanceToPlane(center);
    }

    [[nodiscard]] bool IsOnFrustum(const Frustum& camFrustum, const std::shared_ptr<Transform>& transform) const
    {
        //Get global scale thanks to our transform
        const glm::vec3 globalCenter{ transform->GetModelMatrix() * glm::vec4(center, 1.f) };

        // Scaled orientation
        const glm::vec3 right = transform->GetRight() * extents.x;
        const glm::vec3 up = transform->GetUp() * extents.y;
        const glm::vec3 forward = transform->GetForward() * extents.z;

        const float newIi = std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, right)) +
                            std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, up)) +
                            std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, forward));

        const float newIj = std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, right)) +
                            std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, up)) +
                            std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, forward));

        const float newIk = std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, right)) +
                            std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, up)) +
                            std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, forward));

        const AABB globalAABB(globalCenter, newIi, newIj, newIk);

        return (globalAABB.IsOnOrForwardPlane(camFrustum.leftFace) &&
                globalAABB.IsOnOrForwardPlane(camFrustum.rightFace) &&
                globalAABB.IsOnOrForwardPlane(camFrustum.topFace) &&
                globalAABB.IsOnOrForwardPlane(camFrustum.bottomFace) &&
                globalAABB.IsOnOrForwardPlane(camFrustum.nearFace) &&
                globalAABB.IsOnOrForwardPlane(camFrustum.farFace));
    };
};

class FrustumCulling {
private:
    Frustum frustum{};
    inline static FrustumCulling* frustumCulling;

public:
    FrustumCulling(FrustumCulling &other) = delete;
    void operator=(const FrustumCulling&) = delete;
    virtual ~FrustumCulling();

    static FrustumCulling* GetInstance();

    void UpdateFrustum();
    bool IsOnFrustum(const std::shared_ptr<AABB>& bounds, const std::shared_ptr<Transform>& transform);
    static std::shared_ptr<AABB> GenerateAABB(const std::shared_ptr<Model>& model);

private:
    FrustumCulling();
};


#endif //GLOOMENGINE_FRUSTUMCULLING_H
