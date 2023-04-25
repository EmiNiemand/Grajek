//
// Created by masterktos on 23.03.23.
//

#include "Components/Renderers/CubeMap.h"
#include "glad/glad.h"
#include "stb_image.h"
#include "Shapes/Cube.h"
#include "GloomEngine.h"
#include "EngineManagers/RendererManager.h"

#ifdef DEBUG
#include <tracy/Tracy.hpp>
#endif

#define BASE_PATH "res/textures/"


CubeMap::CubeMap(const std::shared_ptr<GameObject> &parent, int id) : Drawable(parent, id) {
    skyboxMesh = Shape::Cube::GetWithNormals(std::vector<Texture>());
}

CubeMap::~CubeMap() = default;

/**
 * Loads cubemap's source textures.
 * @attention Parameter <b>basePath</b> should have path to folder
 *            containing textures as well as base name for textures. <br/>
 *            Path is relative to <b>res/textures/</b> <br/>
 *            e.g. first texture is in "res/textures/skybox/sky1.jpg"
 *            so <b>basePath</b> should be "skybox/sky" <br/>
 *            Order of textures is: <br/>
 *            <b>RIGHT, LEFT, TOP, BOTTOM, BACK, FRONT</b>
 * @param basePath - path to folder containing textures
 */

void CubeMap::LoadTextures(const std::string& basePath) {
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < 6; i++)
    {
        auto path = BASE_PATH + basePath + std::to_string(i) + ".jpg";

        unsigned char *data = stbi_load(path.c_str(),
                                        &width, &height,
                                        &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else
        {
            printf("Cubemap texture failed to load at path: %s\n", path.c_str());
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    auto shader = RendererManager::GetInstance()->cubeMapShader;
    shader->Activate();
    shader->SetInt("skybox", 5);

    shader = RendererManager::GetInstance()->shader;
    shader->Activate();
    shader->SetInt("skybox", 5);
}

void CubeMap::Update() {
#ifdef DEBUG
    ZoneScopedNC("CubeMap", 0x800080);
#endif
    Drawable::Update();
}

void CubeMap::Draw() {
    auto shader = RendererManager::GetInstance()->cubeMapShader;
    // change depth function so depth test passes
    // when values are equal to depth buffer's content
    glDepthFunc(GL_LEQUAL);
    shader->Activate();

    glBindVertexArray(skyboxMesh->vao);
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    glBindVertexArray(skyboxMesh->vao);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);

    glDepthFunc(GL_LESS); // set depth function back to default
}

// leave it empty or else it's gonna be drawn in shadow map
void CubeMap::Draw(std::shared_ptr<Shader> shader) {}
