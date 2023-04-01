//
// Created by MasterKtos on 28.03.2023.
//

#include "EngineManagers/DebugManager.h"
#include "imgui.h"
#include "EngineManagers/SceneManager.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "windows.h"
#include "psapi.h"

DebugManager::DebugManager() {
    displaySelected = false;
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
        ImGui::Text(selected->GetName().c_str());
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
void DebugManager::processChildren(std::shared_ptr<GameObject> gameObject) {
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
void DebugManager::DisplaySystemInfo() {
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    {
        ImGui::Begin("Usage Info");
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
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