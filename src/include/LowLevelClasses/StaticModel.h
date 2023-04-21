//
// Created by szymo on 31/10/2022.
//

#ifndef OPENGLGP_MODEL_H
#define OPENGLGP_MODEL_H

#include "Mesh.h"
#include "assimp/scene.h"
#include "Model.h"
#include <string>

class StaticModel : public Model
{
public:
    StaticModel(const std::string &path, std::shared_ptr<Shader> &shader, int type = GL_TRIANGLES, bool gamma = false);
    StaticModel(const Mesh &mesh, std::shared_ptr<Shader> &shader, int type = GL_TRIANGLES);

    void LoadModel(std::string const &path) override;

protected:
    void ProcessNode(aiNode *node, const aiScene *scene) override;
    Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene) override;
};


#endif //OPENGLGP_MODEL_H
