#ifndef OPENGLGP_CAMERA_H
#define OPENGLGP_CAMERA_H

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Components/Component.h"
#include "EngineManagers/HIDManager.h"

class Camera : public Component {
private:
    std::shared_ptr<GameObject> target;
    std::shared_ptr<GameObject> player;

public:
    static std::shared_ptr<GameObject> activeCamera;

    glm::vec3 cameraOffset = glm::vec3(0,0,0);
    float zoomLevel = 1.0f; // effective level of zoom. defined as multiplier to the offset on the camera
    float desiredZoomLevel = 1.0f; // zoom level that will be interpolated to
    float parameter = 0.1f;
    bool isZoomed = false;

public:
    Camera(const std::shared_ptr<GameObject> &parent, int id);
    ~Camera() override;

    void Start() override;

    void Update() override;

    void SetTarget(const std::shared_ptr<GameObject> &target);
    void SetZoomLevel(float newZoom);

    glm::mat4 GetViewMatrix();

private:
    glm::vec3 Lerp(glm::vec3 firstPoint, glm::vec3 secondPoint, float parameter);
    float Lerp(float scalar1, float scalar2, float parameter);
};


#endif //OPENGLGP_CAMERA_H
