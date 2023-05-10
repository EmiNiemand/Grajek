#include <utility>
#include <filesystem>
#include "Components/Renderers/Renderer.h"
#include "Utilities.h"
#include "EngineManagers/RendererManager.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "LowLevelClasses/StaticModel.h"
#include "Other/FrustumCulling.h"

#ifdef DEBUG
#include <tracy/Tracy.hpp>
#endif

/**
 * @attention Remember to call LoadModel if you want model to actually display
 */
Renderer::Renderer(const std::shared_ptr<GameObject> &parent, int id) : Drawable(parent, id) {}

Renderer::~Renderer() {
    model.reset();
}

void Renderer::Update() {
#ifdef DEBUG
    ZoneScopedNC("Renderer", 0x800080);
#endif
    if (!parent->isOnFrustum) {
        return;
    }
    Drawable::Update();
}

void Renderer::Draw() {
    if(model == nullptr) return;

    auto shader = RendererManager::GetInstance()->shader;

    shader->Activate();
    shader->SetMat4("model", parent->transform->GetModelMatrix());
    shader->SetVec2("texStrech", textScale);
    shader->SetVec3("material.color", material.color);
    shader->SetFloat("material.shininess", material.shininess);
    shader->SetFloat("material.reflection", material.reflection);
    shader->SetFloat("material.refraction", material.refraction);
    shader->SetBool("isAnimated", false);

    model->Draw();
}

void Renderer::Draw(std::shared_ptr<Shader> shader) {
    if(model == nullptr) return;

    shader->Activate();
    shader->SetMat4("model", parent->transform->GetModelMatrix());
    shader->SetVec2("texStrech", textScale);
    shader->SetVec3("material.color", material.color);
    shader->SetFloat("material.shininess", material.shininess);
    shader->SetFloat("material.reflection", material.reflection);
    shader->SetFloat("material.refraction", material.refraction);
    shader->SetBool("isAnimated", false);

    model->Draw(shader);
}

/**
 * @attention Needs to be called after renderer's constructor
 * @param newPath - relative path starting in res/models/
 */
void Renderer::LoadModel(std::string path) {
    std::string newPath = "res/models/" + path;
    std::filesystem::path normalizedPath(newPath);
    int hash = Utilities::Hash(newPath);

    if (!models.contains(hash)) {
        models.insert({hash, std::make_shared<StaticModel>(normalizedPath.string(), RendererManager::GetInstance()->shader, GL_TRIANGLES)});
    }

    model = models.at(hash);

    parent->bounds = FrustumCulling::GenerateAABB(model);
}

