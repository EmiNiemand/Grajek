//
// Created by masterktos on 23.03.23.
//

#ifndef GLOOMENGINE_CUBEMAP_H
#define GLOOMENGINE_CUBEMAP_H

#include <string>
#include <memory>
#include "glm/ext/matrix_float4x4.hpp"
#include "LowLevelClasses/Shader.h"
#include "LowLevelClasses/Mesh.h"
#include "Components/Renderers/Drawable.h"

class CubeMap : public Drawable {
private:
    unsigned int textureID;
    std::shared_ptr<Mesh> skyboxMesh;
public:
    CubeMap(const std::shared_ptr<GameObject> &parent, int id);
    ~CubeMap() override;

    void LoadTextures(const std::string& basePath);

    void Update() override;
    void OnDestroy() override;

    void Draw() override;
    void Draw(std::shared_ptr<Shader> shader) override;
};


#endif //GLOOMENGINE_CUBEMAP_H
