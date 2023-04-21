//
// Created by MasterKtos on 21.04.2023.
//

#ifndef GLOOMENGINE_MODEL_H
#define GLOOMENGINE_MODEL_H

#include "Mesh.h"
#include "assimp/scene.h"
#include <string>

class Model {
protected:
    // model data
    std::vector<Texture> texturesLoaded;
    std::vector<Mesh> meshes;
    std::string directory;
    std::shared_ptr<Shader> shader;
    // GL_TRIANGLES etc
    int type;
    bool gammaCorrection;
public:
    Model(std::string const &path, std::shared_ptr<Shader> &shader, int type = GL_TRIANGLES, bool gamma = false);
    Model(const Mesh& mesh, std::shared_ptr<Shader> &shader, int type = GL_TRIANGLES);
    virtual void Draw();
    virtual void Draw(std::shared_ptr<Shader> useShader);

protected:
    static unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma = false);
    void LoadModel(std::string const &path);
    virtual void ProcessNode(aiNode *node, const aiScene *scene);

    virtual Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> LoadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};


#endif //GLOOMENGINE_MODEL_H
