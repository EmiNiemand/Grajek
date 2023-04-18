#include <utility>

#include "Components/Renderers/Renderer.h"
#include "Utilities.h"
#include "EngineManagers/RendererManager.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "LowLevelClasses/Model.h"
#include <filesystem>

/**
 * @attention Remember to call LoadModel if you want model to actually display
 */
Renderer::Renderer(const std::shared_ptr<GameObject> &parent, int id) : Drawable(parent, id) {}

Renderer::~Renderer() {
    model.reset();
}

void Renderer::Update() {
    Drawable::Update();
}

void Renderer::Draw() {
    if(model == nullptr) return;

    std::shared_ptr<Shader> shader;
    if (parent->GetName() == "House" || parent->GetName() == "House1" || parent->GetName() == "House2" || parent->GetName() == "House3") {
        if (GloomEngine::GetInstance()->i == 0) {
            shader = RendererManager::GetInstance()->shader;
            shader->Activate();
            shader->SetMat4("model", parent->transform->GetModelMatrix());
            shader->SetVec3("material.color", material.color);
            shader->SetFloat("material.shininess", material.shininess);
            shader->SetFloat("material.reflection", material.reflection);
            shader->SetFloat("material.refraction", material.refraction);
        } else if (GloomEngine::GetInstance()->i == 1) {
            shader = RendererManager::GetInstance()->shader1;
            shader->Activate();
            shader->SetMat4("model", parent->transform->GetModelMatrix());
            shader->SetVec3("material.color", material.color);
            shader->SetFloat("material.shininess", material.shininess);
            shader->SetFloat("material.reflection", material.reflection);
            shader->SetFloat("material.refraction", material.refraction);
        } else if (GloomEngine::GetInstance()->i == 2) {
            shader = RendererManager::GetInstance()->shader2;
            shader->Activate();
            shader->SetMat4("model", parent->transform->GetModelMatrix());
            shader->SetVec3("material.color", material.color);
            shader->SetFloat("material.shininess", material.shininess);
            shader->SetFloat("material.reflection", material.reflection);
            shader->SetFloat("material.refraction", material.refraction);
        } else if (GloomEngine::GetInstance()->i == 3) {
            shader = RendererManager::GetInstance()->shader3;
            shader->Activate();
            shader->SetMat4("model", parent->transform->GetModelMatrix());
            shader->SetVec3("material.color", material.color);
            shader->SetFloat("material.shininess", material.shininess);
            shader->SetFloat("material.reflection", material.reflection);
            shader->SetFloat("material.refraction", material.refraction);
        }
    } else {
        shader = RendererManager::GetInstance()->shader;
        shader->Activate();
        shader->SetMat4("model", parent->transform->GetModelMatrix());
        shader->SetVec3("material.color", material.color);
        shader->SetFloat("material.shininess", material.shininess);
        shader->SetFloat("material.reflection", material.reflection);
        shader->SetFloat("material.refraction", material.refraction);
    }

    if (parent->GetName() == "House" || parent->GetName() == "House1" || parent->GetName() == "House2" || parent->GetName() == "House3") {
        GloomEngine::GetInstance()->i++;
    }

    model->Draw();
}

void Renderer::Draw(std::shared_ptr<Shader> shader) {
    if(model == nullptr) return;

    shader->Activate();
    shader->SetMat4("model", parent->transform->GetModelMatrix());
    shader->SetVec3("material.color", material.color);
    shader->SetFloat("material.shininess", material.shininess);
    shader->SetFloat("material.reflection", material.reflection);
    shader->SetFloat("material.refraction", material.refraction);

    model->Draw(shader);
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

