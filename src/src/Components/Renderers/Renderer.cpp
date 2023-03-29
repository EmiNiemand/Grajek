#include <utility>

#include "Components/Renderers/Renderer.h"
#include "GloomEngine.h"
#include "Utilities.h"
#include "EngineManagers/RendererManager.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "LowLevelClasses/Model.h"
#include "Components/Renderers/Camera.h"
#include <filesystem>

std::map<uint32_t, std::shared_ptr<Model>> Renderer::models;

/**
 * @attention Remember to call LoadModel if you want model to actually display
 */
Renderer::Renderer(const std::shared_ptr<GameObject> &parent, int id) : Component(parent, id) {
    objectColor = {1.0f, 1.0f, 1.0f};
    shininess = 32.0f;
}

Renderer::~Renderer() {
    model.reset();
}

void Renderer::Update() {
    Draw();
    Component::Update();
}

/**
 * @attention Needs to be called after renderer's constructor
 * @param newPath - relative path starting in res/models/
 */
void Renderer::LoadModel(std::string path) {
    std::string newPath = "res/models/" + path;
    std::filesystem::path normalizedPath(newPath);
    uint32_t hash = Utilities::Hash(newPath);

    if (!models.contains(hash)) {
        models.insert({hash, std::make_shared<Model>( normalizedPath.string(), RendererManager::GetInstance()->shader, GL_TRIANGLES)});
    }

    model = models.at(hash);
}

void Renderer::Draw() {
    if(model == nullptr) return;

    auto shader = RendererManager::GetInstance()->shader;

    shader->Activate();
    shader->SetMat4("model", parent->transform->GetModelMatrix());
    shader->SetFloat("shininess", shininess);
    shader->SetVec3("objectColor", objectColor);
    model->Draw();
}


