//
// Created by MasterKtos on 28.03.2023.
//

#include "EngineManagers/HierarchyManager.h"
#include "imgui.h"
#include "EngineManagers/SceneManager.h"
#include "GameObjectsAndPrefabs/GameObject.h"



HierarchyManager* HierarchyManager::hierarchyManager = nullptr;

HierarchyManager::HierarchyManager() {
    displaySelected = false;
}
HierarchyManager::~HierarchyManager() {}

HierarchyManager* HierarchyManager::GetInstance() {
	if (hierarchyManager == nullptr) {
		hierarchyManager = new HierarchyManager();
	}
	return hierarchyManager;
}

void HierarchyManager::Initialize(GLFWwindow* window, const char* glsl_version) {
	ImGui::CreateContext();

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
}

void HierarchyManager::Render() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    {
        ImGui::Begin("TurboNiuchacz 3000");
        ImGui::Text(SceneManager::GetInstance()->activeScene.get()->GetName().c_str());
        ImGui::SameLine();
        if (ImGui::SmallButton("Open")) {
            displaySelected = true;
            selected = SceneManager::GetInstance()->activeScene;
        }
        ImGui::Indent();
        for (const auto& child : SceneManager::GetInstance()->activeScene.get()->children) {
            ImGui::Text(child.second->GetName().c_str());
            ImGui::SameLine();
            if (ImGui::SmallButton("Open")) {
                displaySelected = true;
                selected = child.second;
            }
            processChildren(child.second);
        }
        ImGui::Unindent();
        ImGui::End();
    }

    if (displaySelected) {
        glm::vec3 vector1;
        glm::vec3 vector2;
        static bool inputBool;
        static float inputVector1[3] = {0.0f,0.0f,0.0f};
        static float inputVector2[3] = {0.0f,0.0f,0.0f};
        ImGui::Begin("Properties");
        ImGui::InputFloat3("inputVector1", inputVector1);
        ImGui::InputFloat3("inputVector1", inputVector2);
        ImGui::Checkbox("inputBool", &inputBool);
        if (ImGui::Button("Close"))
        {
            displaySelected = false;
        }
        ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
void HierarchyManager::processChildren(std::shared_ptr<GameObject> gameObject) {
    if (gameObject.get()->children.empty())
        return;
    ImGui::Indent();
    for (const auto& child : gameObject->children)
    {
        ImGui::Text(child.second->GetName().c_str());
        ImGui::SameLine();
        if (ImGui::SmallButton("Open")) {
            displaySelected = true;
            selected = child.second;
        }
        processChildren(child.second);
    }
    ImGui::Unindent();
}