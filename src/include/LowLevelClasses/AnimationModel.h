//
// Created by szymo on 31/10/2022.
//

#ifndef OPENGLGP_MODEL_H
#define OPENGLGP_MODEL_H

#include "Mesh.h"
#include "assimp/scene.h"
#include<glm/gtc/quaternion.hpp>
#include <string>
#include <map>

struct BoneInfo
{
    /*id is index in finalBoneMatrices*/
    int id;

    /*offset matrix transforms vertex from model space to bone space*/
    glm::mat4 offset;
};

class AnimationModel
{
private:
    std::map<std::string, BoneInfo> boneInfoMap;
    uint16_t boneCounter = 0;

    // model data
    std::vector<Texture> texturesLoaded;
    std::vector<Mesh> meshes;
    std::string directory;
    std::shared_ptr<Shader> shader;
	// GL_TRIANGLES etc
    int type;
    bool gammaCorrection;

public:
    AnimationModel(std::string const &path, std::shared_ptr<Shader> &shader, int type = GL_TRIANGLES, bool gamma = false);
    AnimationModel(Mesh mesh, std::shared_ptr<Shader> &shader, int type = GL_TRIANGLES);
    void Draw();
    void Draw(std::shared_ptr<Shader> useShader);
    auto& GetBoneInfoMap() { return boneInfoMap; }
    uint16_t& GetBoneCount() { return boneCounter; }

private:
    unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma = false);
    void LoadModel(std::string const &path);
    void ProcessNode(aiNode *node, const aiScene *scene);
    void SetVertexBoneDataToDefault(Vertex& vertex);
    Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene);
    void SetVertexBoneData(Vertex& vertex, int boneID, float weight);
    void ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> LoadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};


#endif //OPENGLGP_MODEL_H
