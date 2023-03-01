//
// Created by szymo on 10/10/2022.
//

#ifndef OPENGLGP_SHADER_H
#define OPENGLGP_SHADER_H

#include "glad/glad.h"
#include "glm/matrix.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <string>

class Shader {
private:
    GLuint shader = -1;
public:
    Shader(std::string vertexShaderSource, std::string fragmentShaderSource, std::string geometryShaderSource = "");

    [[nodiscard]] GLuint GetShader() const;

    void Activate();
    void Delete();

    void SetBool(const std::string &name, bool value) const;
    void SetInt(const std::string &name, int value) const;
    void SetFloat(const std::string &name, float value) const;
    void SetVec2(const std::string &name, const glm::vec2 &value) const;
    void SetVec2(const std::string &name, float x, float y) const;
    void SetVec3(const std::string &name, const glm::vec3 &value) const;
    void SetVec3(const std::string &name, float x, float y, float z) const;
    void SetVec4(const std::string &name, const glm::vec4 &value) const;
    void SetVec4(const std::string &name, float x, float y, float z, float w);
    void SetMat2(const std::string &name, const glm::mat2 &mat) const;
    void SetMat3(const std::string &name, const glm::mat3 &mat) const;
    void SetMat4(const std::string &name, const glm::mat4 &mat) const;

private:
    static void LoadShader(std::string& shaderPath, std::string& shaderCodeOut);
};


#endif //OPENGLGP_SHADER_H
