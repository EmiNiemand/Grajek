#ifndef OPENGLGP_DIRECTIONALLIGHT_H
#define OPENGLGP_DIRECTIONALLIGHT_H

#include "glm/matrix.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Components/Component.h"

class DirectionalLight : public Component {
private:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    glm::vec3 color;

public:
    DirectionalLight(const std::shared_ptr<GloomEngine> &gloomEngine, const std::shared_ptr<GameObject> &parent, int id);
    ~DirectionalLight() override;

    const glm::vec3 &GetAmbient() const;
    void SetAmbient(const glm::vec3 &ambient);

    const glm::vec3 &GetDiffuse() const;

    void SetDiffuse(const glm::vec3 &diffuse);
    const glm::vec3 &GetSpecular() const;

    void SetSpecular(const glm::vec3 &specular);

    const glm::vec3 &GetColor() const;
    void SetColor(const glm::vec3 &color);
};


#endif //OPENGLGP_DIRECTIONALLIGHT_H
