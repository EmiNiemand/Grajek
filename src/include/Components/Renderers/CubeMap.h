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
#include "Components/Component.h"

//TODO: this should probably be at least partially moved to low-level classes
class CubeMap : public Component {
private:
    unsigned int textureID;
    std::shared_ptr<Mesh> skyboxMesh;
public:
    CubeMap(const std::shared_ptr<GloomEngine> &gloomEngine,
            const std::shared_ptr<GameObject> &parent, int id);

    void LoadTextures(const std::string& basePath);

    void Update() override;

    void Draw();
};


#endif //GLOOMENGINE_CUBEMAP_H
