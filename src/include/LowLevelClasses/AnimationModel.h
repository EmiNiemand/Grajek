//
// Created by szymo on 31/10/2022.
//

#ifndef OPENGLGP_MODEL_H
#define OPENGLGP_MODEL_H

#include "Model.h"
#include "Mesh.h"
#include "assimp/scene.h"
#include<glm/gtc/quaternion.hpp>
#include <string>
#include <unordered_map>

struct BoneInfo
{
    /*id is index in finalBoneMatrices*/
    int id;

    /*offset matrix transforms vertex from model space to bone space*/
    glm::mat4 offset;
};

class AnimationModel : public Model
{
private:
    std::unordered_map<std::string, BoneInfo> boneInfoMap;
    uint16_t boneCounter = 0;

public:
    AnimationModel(const std::string &path, std::shared_ptr<Shader> &shader, int type = GL_TRIANGLES, bool gamma = false);
    AnimationModel(const Mesh &mesh, std::shared_ptr<Shader> &shader, int type = GL_TRIANGLES);

    // Getters
    std::unordered_map<std::string, BoneInfo>& GetBoneInfoMap();
    uint16_t& GetBoneCount();

private:
    static void SetVertexBoneDataToDefault(Vertex& vertex);
    Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene) override;
    static void SetVertexBoneData(Vertex& vertex, int boneID, float weight);
    void ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene);
};


#endif //OPENGLGP_MODEL_H
