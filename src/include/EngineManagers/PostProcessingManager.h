//
// Created by szymo on 04/04/2023.
//

#ifndef GLOOMENGINE_POSTPROCESSINGMANAGER_H
#define GLOOMENGINE_POSTPROCESSINGMANAGER_H

#include <memory>

class Shader;

class PostProcessingManager{
private:
    // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
    float quadVertices[24] = {
            // positions   // texCoords
            -1.0f,  1.0f,  0.0f, 1.0f,
            -1.0f, -1.0f,  0.0f, 0.0f,
            1.0f, -1.0f,  1.0f, 0.0f,
            -1.0f,  1.0f,  0.0f, 1.0f,
            1.0f, -1.0f,  1.0f, 0.0f,
            1.0f,  1.0f,  1.0f, 1.0f
    };

    inline static PostProcessingManager* postProcessingManager;

    std::shared_ptr<Shader> postProcessingShader;
    unsigned int quadVAO, quadVBO;
    unsigned int rbo;

public:
    unsigned int framebuffer;
    unsigned int textureColorBuffer;

public:
    PostProcessingManager(PostProcessingManager &other) = delete;
    void operator=(const PostProcessingManager&) = delete;
    virtual ~PostProcessingManager();

    static PostProcessingManager* GetInstance();

    void DrawBuffer();
    void Free() const;

private:
    explicit PostProcessingManager();

};


#endif //GLOOMENGINE_POSTPROCESSINGMANAGER_H
