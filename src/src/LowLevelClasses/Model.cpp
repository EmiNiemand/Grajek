//
// Created by MasterKtos on 21.04.2023.
//

#include "LowLevelClasses/Model.h"

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "glad/glad.h"
#include "stb_image.h"
#include "spdlog/spdlog.h"
#include <filesystem>

unsigned int Model::TextureFromFile(const char *path, const std::string &textureDir, bool gamma) {
    std::string filename = std::string(path);
    filename = textureDir + '/' + filename;
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    } else {
        spdlog::info("Texture failed to load at path: " + (std::string)path);
        stbi_image_free(data);
    }
    return textureID;
}

Model::Model(std::string const &path, std::shared_ptr<Shader> &shader,
             int type, bool gamma) : shader(shader), type(type), gammaCorrection(gamma) {}

Model::Model(const Mesh& mesh, std::shared_ptr<Shader> &shader,
             int type) : shader(shader), type(type), gammaCorrection(false) {}


Model::~Model() {
    for (int i = 0; i < texturesLoaded.size(); ++i) {
        glDeleteTextures(1, &texturesLoaded[i].id);
    }
}

void Model::Draw() {
    for(auto & mesh : meshes) mesh.Draw(shader, type);
}

void Model::Draw(std::shared_ptr<Shader> useShader) {
    for(auto & mesh : meshes) mesh.Draw(useShader, type);
}

std::vector<Texture> Model::LoadMaterialTextures(aiMaterial *mat, aiTextureType textureType, std::string typeName) {
    std::vector<Texture> textures;
    for(unsigned int i = 0; i < mat->GetTextureCount(textureType); i++)
    {
        aiString str;
        mat->GetTexture(textureType, i, &str);
        std::string path = std::filesystem::path(str.C_Str()).generic_string();
        path = path.substr(path.find_last_of('/') + 1);
        // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
        bool skip = false;
        for(auto & texture : texturesLoaded)
        {
            if(std::strcmp(texture.path.data(), path.c_str()) == 0)
            {
                textures.push_back(texture);
                skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                break;
            }
        }
        if(!skip)
        {   // if texture hasn't been loaded already, load it
            Texture texture;
            texture.id = TextureFromFile(path.c_str(), this->directory);
            texture.type = typeName;
            texture.path = path;
            textures.push_back(texture);
            texturesLoaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
        }
    }
    return textures;
}
