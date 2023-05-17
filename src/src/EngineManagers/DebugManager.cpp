//
// Created by MasterKtos on 28.03.2023.
//
#ifdef DEBUG
#include "EngineManagers/DebugManager.h"
#include "imgui.h"
#include "EngineManagers/SceneManager.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "windows.h"
#include "psapi.h"
#include <string.h>
#include <glm/gtc/type_ptr.hpp>
#include <filesystem>

DebugManager::DebugManager() {
    displaySelected = false;
    transformExtracted = false;
}
DebugManager::~DebugManager() = default;

DebugManager* DebugManager::GetInstance() {
	if (hierarchyManager == nullptr) {
		hierarchyManager = new DebugManager();
	}
	return hierarchyManager;
}

void DebugManager::Initialize(GLFWwindow* window, const char* glsl_version) {
	ImGui::CreateContext();

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
}


void DebugManager::Render() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    DisplaySystemInfo();
    SaveMenu();
    {
        ImGui::Begin("Debug Window");

        ImGui::Text("Hierarchy Tree");
        ImGui::Text(SceneManager::GetInstance()->activeScene->GetName().c_str());
        ImGui::SameLine();
        if (ImGui::SmallButton("Open")) {
            displaySelected = true;
            selected = SceneManager::GetInstance()->activeScene;
        }
        ImGui::Indent();
        std::string label;
        for (const auto& child : SceneManager::GetInstance()->activeScene->children) {
            label = "Open##" + std::to_string(child.first);
            ImGui::Text(child.second->GetName().c_str());
            ImGui::SameLine();
            if (ImGui::SmallButton(label.c_str())) {
                displaySelected = true;
                transformExtracted = false;
                selected = child.second;
            }
            ProcessChildren(child.second);
        }
        ImGui::Unindent();
        ImGui::End();
    }

    if (displaySelected) {
        static bool inputBool;
        static float inputVector1[3] = {0.0f,0.0f,0.0f};
        static float inputVector2[3] = { 0.0f,0.0f,0.0f };
        static float inputVector3[3] = { 0.0f,0.0f,0.0f };
        glm::vec3 positionHolder;
        glm::vec3 rotationHolder;
        glm::vec3 scaleHolder;


        positionHolder = selected->transform->GetLocalPosition();
        rotationHolder = selected->transform->GetLocalRotation();
        scaleHolder = selected->transform->GetLocalScale();
        if (!transformExtracted) {
            ExtractVec3ToFloat3(positionHolder, inputVector1);
            ExtractVec3ToFloat3(rotationHolder, inputVector2);
            ExtractVec3ToFloat3(scaleHolder, inputVector3);
            transformExtracted = true;
        }
        positionHolder = InjectFloat3IntoVec3(inputVector1);
        rotationHolder = InjectFloat3IntoVec3(inputVector2);
        scaleHolder = InjectFloat3IntoVec3(inputVector3);

        selected->transform->SetLocalPosition(positionHolder);
        selected->transform->SetLocalRotation(rotationHolder);
        selected->transform->SetLocalScale(scaleHolder);


        ImGui::Begin("Properties");
        ImGui::Text(selected->GetName().c_str());
        ImGui::DragFloat3("Position", inputVector1, 1.0f);
        ImGui::DragFloat3("Rotation", inputVector2, 1.0f, 0.0f,360.0f);
        ImGui::DragFloat3("Scale", inputVector3, 1.0f, 0.0f,10.0f);
        ImGui::Checkbox("inputBool", &inputBool);
        if (ImGui::Button("Close"))
        {
            displaySelected = false;
            transformExtracted = false;
        }
        ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
void DebugManager::ProcessChildren(std::shared_ptr<GameObject> gameObject) {
    if (gameObject->children.empty())
        return;
    ImGui::Indent();
    std::string label;
    for (const auto& child : gameObject->children)
    {
        label = "Open##" + std::to_string(child.first);
        ImGui::Text(child.second->GetName().c_str());
        ImGui::SameLine();
        if (ImGui::SmallButton(label.c_str())) {
            displaySelected = true;
            transformExtracted = false;
            selected = child.second;
        }
        ProcessChildren(child.second);
    }
    ImGui::Unindent();
}
void DebugManager::ExtractVec3ToFloat3(glm::vec3 input,float* output)
{
    output[0] = input.x;
    output[1] = input.y;
    output[2] = input.z;
}

glm::vec3 DebugManager::InjectFloat3IntoVec3(float* input)
{
    return glm::make_vec3(input);
}

void DebugManager::DisplaySystemInfo() {
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    {
        ImGui::Begin("Usage Info");
        ImGui::Text("Update: %.3f ms/frame (%.1f FPS)", 1000.0f * GloomEngine::GetInstance()->deltaTime, 1 /
            GloomEngine::GetInstance()->deltaTime);
        ImGui::Text("FixedUpdate: %.3f ms/frame (%.1f FPS)", 1000.0f * GloomEngine::GetInstance()->fixedDeltaTime, 1 /
            GloomEngine::GetInstance()->fixedDeltaTime);
        ImGui::Text("AIUpdate: %.3f ms/frame (%.1f FPS)", 1000.0f * GloomEngine::GetInstance()->AIDeltaTime, 1 /
            GloomEngine::GetInstance()->AIDeltaTime);
#ifdef DEBUG
        ImGui::Text("MainLoop: %.3f ms/frame (%.1f FPS)", 1000.0f * GloomEngine::GetInstance()->engineDeltaTime, 1 /
            GloomEngine::GetInstance()->engineDeltaTime);
#endif
        MEMORYSTATUSEX memInfo;
        memInfo.dwLength = sizeof(MEMORYSTATUSEX);
        GlobalMemoryStatusEx(&memInfo);
        DWORDLONG totalVirtualMem = memInfo.ullTotalPageFile;
        ImGui::Text("Total Virtual Memory: %s Mb", std::to_string(totalVirtualMem / 100000).c_str());
        PROCESS_MEMORY_COUNTERS_EX pmc;
        GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
        SIZE_T virtualMemUsedByMe = pmc.PrivateUsage;
        ImGui::Text("VMemory Used: %s Mb", std::to_string(virtualMemUsedByMe / 100000).c_str());

        DWORDLONG totalPhysMem = memInfo.ullTotalPhys;
        ImGui::Text("Total Physical Memory: %s Mb", std::to_string(totalPhysMem / 100000).c_str());

        SIZE_T physMemUsedByMe = pmc.WorkingSetSize;
        ImGui::Text("Physical Memory Usage: %s Mb", std::to_string(physMemUsedByMe / 100000).c_str());
        ImGui::End();
    }
}

void DebugManager::SaveMenu()
{
    ImGui::Begin("Save Menu");
    if (ImGui::SmallButton("Save")) {
        std::filesystem::path path = std::filesystem::current_path();
        path /= "res";
        path /= "ProjectConfig";

        SceneManager::GetInstance()->SaveStaticObjects(path.string(),"map0");
    }
    if (ImGui::SmallButton("Add new house")){
        SceneManager::GetInstance()->CreatePrefabObject("House");
    }
    ImGui::End();
}

void DebugManager::Free() const {

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
#endif
