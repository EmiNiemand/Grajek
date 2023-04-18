//
// Created by MasterKtos on 28.03.2023.
//

#include "EngineManagers/DebugManager.h"
#include "imgui.h"
#include "EngineManagers/SceneManager.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "windows.h"
#include "psapi.h"
#include <string.h>
#include <glm/gtc/type_ptr.hpp>

DebugManager::DebugManager() {
    displaySelected = false;
    positionExtracted = false;
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
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
}


void DebugManager::Render() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    DisplaySystemInfo();

    {
        ImGui::Begin("Debug Window");

        ImGui::Text("Hierarchy Tree");
        ImGui::Text(SceneManager::GetInstance()->activeScene.get()->GetName().c_str());
        ImGui::SameLine();
        if (ImGui::SmallButton("Open")) {
            displaySelected = true;
            selected = SceneManager::GetInstance()->activeScene;
        }
        ImGui::Indent();
        std::string label;
        for (const auto& child : SceneManager::GetInstance()->activeScene.get()->children) {
            label = "Open##" + std::to_string(child.first);
            ImGui::Text(child.second->GetName().c_str());
            ImGui::SameLine();
            if (ImGui::SmallButton(label.c_str())) {
                displaySelected = true;
                positionExtracted = false;
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
        glm::vec3 positionHolder;
        static float inputVector2[3] = {0.0f,0.0f,0.0f};

        positionHolder = selected->transform.get()->GetLocalPosition();
        if (!positionExtracted) {
            ExtractVec3ToFloat3(positionHolder, inputVector1);
            positionExtracted = true;
        }
        positionHolder = InjectFloat3IntoVec3(inputVector1);

        selected->transform.get()->SetLocalPosition(positionHolder);


        ImGui::Begin("Properties");
        ImGui::Text(selected->GetName().c_str());
        ImGui::InputFloat3("Position", inputVector1);
        ImGui::InputFloat3("inputVector2", inputVector2);
        ImGui::Checkbox("inputBool", &inputBool);
        if (ImGui::Button("Close"))
        {
            displaySelected = false;
            positionExtracted = false;
        }
        ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
void DebugManager::ProcessChildren(std::shared_ptr<GameObject> gameObject) {
    if (gameObject.get()->children.empty())
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
            positionExtracted = false;
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
        ImGui::Text("MainLoop: %.3f ms/frame (%.1f FPS)", 1000.0f * GloomEngine::GetInstance()->engineDeltaTime, 1 /
            GloomEngine::GetInstance()->engineDeltaTime);
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

void DebugManager::Free() const {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
