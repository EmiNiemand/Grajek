//
// Created by szymo on 10/10/2022.
//

#include "LowLevelClasses/Shader.h"
#include "spdlog/spdlog.h"

#include <sstream>
#include <fstream>

// Prefix for any path given as shader source
#define BASE_PATH "res/shaders/"

Shader::Shader(std::string vertexShaderSource, std::string fragmentShaderSource, std::string geometryShaderSource) {

    std::string vCode;
    LoadShader(vertexShaderSource, vCode);
    const GLchar* cvCode = vCode.c_str();
    // Generate Shader object
    // Create Vertex Shader Object and get its reference
    GLuint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // Attach Vertex Shader source to the Vertex Shader Object
    glShaderSource(vertexShader, 1, &cvCode, nullptr);
    // Compile the Vertex Shader into machine code
    glCompileShader(vertexShader);

    std::string fCode;
    LoadShader(fragmentShaderSource, fCode);
    const GLchar* cfCode = fCode.c_str();
    // Create Fragment Shader Object and get its reference
    GLuint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // Attach Fragment Shader source to the Fragment Shader Object
    glShaderSource(fragmentShader, 1, &cfCode, nullptr);
    // Compile the Vertex Shader into machine code
    glCompileShader(fragmentShader);

    GLuint geometryShader;
    if(!geometryShaderSource.empty())
    {
        std::string gCode;
        LoadShader(geometryShaderSource, gCode);
        const GLchar* gfCode = gCode.c_str();
        // Create Fragment Shader Object and get its reference
        geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
        // Attach Fragment Shader source to the Fragment Shader Object
        glShaderSource(geometryShader, 1, &gfCode, nullptr);
        // Compile the Vertex Shader into machine code
        glCompileShader(geometryShader);
    }


    shader = glCreateProgram();
    // Attach the Vertex and Fragment Shaders to the Shader Program
    glAttachShader(shader, vertexShader);
    glAttachShader(shader, fragmentShader);
    if(geometryShaderSource != ""){
        glAttachShader(shader, geometryShader);
    }
    // Wrap-up/Link all the shaders together into the Shader Program
    glLinkProgram(shader);

    // Delete the now useless Vertex and Fragment Shader objects
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    if(geometryShaderSource != ""){
        glDeleteShader(geometryShader);
    }
}


void Shader::LoadShader(std::string& shaderPath, std::string& shaderCode) {
    std::ifstream ShaderFile;

    ShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        ShaderFile.open(BASE_PATH + shaderPath);
        std::stringstream ShaderStream;

        ShaderStream << ShaderFile.rdbuf();
        ShaderFile.close();

        shaderCode = ShaderStream.str();
    } catch (std::ifstream::failure& e) {
        spdlog::error("Shader file loading failure");
    }
}


GLuint Shader::GetShader() const {
    return shader;
}

void Shader::Activate() {
    glUseProgram(shader);
}


void Shader::Delete() {
    glDeleteProgram(shader);
}

#pragma region Utils
void Shader::SetBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(shader, name.c_str()), (int)value);
}

void Shader::SetInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(shader, name.c_str()), value);
}

void Shader::SetFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(shader, name.c_str()), value);
}

void Shader::SetVec2(const std::string &name, const glm::vec2 &value) const
{
    glUniform2fv(glGetUniformLocation(shader, name.c_str()), 1, &value[0]);
}
void Shader::SetVec2(const std::string &name, float x, float y) const
{
    glUniform2f(glGetUniformLocation(shader, name.c_str()), x, y);
}

void Shader::SetVec3(const std::string &name, const glm::vec3 &value) const
{
    glUniform3fv(glGetUniformLocation(shader, name.c_str()), 1, &value[0]);
}
void Shader::SetVec3(const std::string &name, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(shader, name.c_str()), x, y, z);
}

void Shader::SetVec4(const std::string &name, const glm::vec4 &value) const
{
    glUniform4fv(glGetUniformLocation(shader, name.c_str()), 1, &value[0]);
}
void Shader::SetVec4(const std::string &name, float x, float y, float z, float w)
{
    glUniform4f(glGetUniformLocation(shader, name.c_str()), x, y, z, w);
}

void Shader::SetMat2(const std::string &name, const glm::mat2 &mat) const
{
    glUniformMatrix2fv(glGetUniformLocation(shader, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::SetMat3(const std::string &name, const glm::mat3 &mat) const
{
    glUniformMatrix3fv(glGetUniformLocation(shader, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::SetMat4(const std::string &name, const glm::mat4 &mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(shader, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

#pragma endregion