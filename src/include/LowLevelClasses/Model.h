//
// Created by szymo on 31/10/2022.
//

#ifndef OPENGLGP_MODEL_H
#define OPENGLGP_MODEL_H

#include "Mesh.h"
#include "assimp/scene.h"
#include <string>

unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma = false);

class Model
{
private:
    // model data
    std::vector<Texture> texturesLoaded;
    std::vector<Mesh> meshes;
    std::string directory;
    Shader* shader;
    int type;
    bool gammaCorrection;
public:
    Model(std::string const &path, Shader* shader, int type, bool gamma = false);
    Model(Mesh mesh, Shader* shader, int type);
    void Draw();
    void DrawInstanced(int amount);
    Shader *GetShader() const;

    const std::vector<Texture> &GetTexturesLoaded() const;

    const std::vector<Mesh> &GetMeshes() const;

private:
    void LoadModel(std::string const &path);
    void ProcessNode(aiNode *node, const aiScene *scene);
    Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> LoadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};


#endif //OPENGLGP_MODEL_H
