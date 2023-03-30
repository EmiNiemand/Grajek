//
// Created by MasterKtos on 28.03.2023.
//

#include "EngineManagers/HierarchyManager.h"
#include "imgui.h"

HierarchyManager* HierarchyManager::hierarchyManager = nullptr;

HierarchyManager::HierarchyManager() {}
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
        ImGui::Begin("Hello, world!");
        ImGui::Text("This is some useful text.");
        ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}