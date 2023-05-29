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
#include "Components/Renderers/Renderer.h"
#include "Components/PhysicsAndColliders/BoxCollider.h"
#include <filesystem>

namespace fs = std::filesystem;

DebugManager::DebugManager() {
    displaySelected = false;
    transformExtracted = false;
    safetySwitch = false;
    modelPaths = FindModelPaths();
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
        ImGui::Text("%s", SceneManager::GetInstance()->activeScene->GetName().c_str());
        ImGui::SameLine();
        if (ImGui::SmallButton("Open")) {
            displaySelected = true;
            safetySwitch = false;
            selected = SceneManager::GetInstance()->activeScene;
        }
        ImGui::Indent();
        std::string label;
        for (const auto& child : SceneManager::GetInstance()->activeScene->children) {
            label = "Open##" + std::to_string(child.first);
            ImGui::Text("%s", child.second->GetName().c_str());
            ImGui::SameLine();
            if (ImGui::SmallButton(label.c_str())) {
                displaySelected = true;
                transformExtracted = false;
                selected = child.second;
                safetySwitch = false;
            }
            ProcessChildren(child.second);
        }
        ImGui::Unindent();
        ImGui::End();
    }

    if (displaySelected) {
        static float inputVector1[3] = {0.0f,0.0f,0.0f};
        static float inputVector2[3] = { 0.0f,0.0f,0.0f };
        static float inputVector3[3] = { 0.0f,0.0f,0.0f };
        static float inputVector4[3] = { 0.0f,0.0f,0.0f };
        static float inputVector5[3] = { 0.0f,0.0f,0.0f };
        glm::vec3 positionHolder;
        glm::vec3 rotationHolder;
        glm::vec3 scaleHolder;
        glm::vec3 coliderSizeHolder;
        glm::vec3 coliderOffsetHolder;


        positionHolder = selected->transform->GetLocalPosition();
        rotationHolder = selected->transform->GetLocalRotation();
        scaleHolder = selected->transform->GetLocalScale();
        if(selected->GetComponent<BoxCollider>()){
            coliderSizeHolder = selected->GetComponent<BoxCollider>()->GetSize();
            coliderOffsetHolder = selected->GetComponent<BoxCollider>()->GetOffset();
        }
        if (!transformExtracted) {
            ExtractVec3ToFloat3(positionHolder, inputVector1);
            ExtractVec3ToFloat3(rotationHolder, inputVector2);
            ExtractVec3ToFloat3(scaleHolder, inputVector3);
            if(selected->GetComponent<BoxCollider>()){
                ExtractVec3ToFloat3(coliderSizeHolder,inputVector4);
                ExtractVec3ToFloat3(coliderOffsetHolder,inputVector5);
            }
            transformExtracted = true;
        }
        positionHolder = InjectFloat3IntoVec3(inputVector1);
        rotationHolder = InjectFloat3IntoVec3(inputVector2);
        scaleHolder = InjectFloat3IntoVec3(inputVector3);
        coliderSizeHolder = InjectFloat3IntoVec3(inputVector4);
        coliderOffsetHolder = InjectFloat3IntoVec3(inputVector5);


        selected->transform->SetLocalPosition(positionHolder);
        selected->transform->SetLocalRotation(rotationHolder);
        selected->transform->SetLocalScale(scaleHolder);
        if(selected->GetComponent<BoxCollider>()){
            selected->GetComponent<BoxCollider>()->SetSize(coliderSizeHolder);
            selected->GetComponent<BoxCollider>()->SetOffset(coliderOffsetHolder);
        }

        ImGui::Begin("Properties");
        ImGui::Text("%s", selected->GetName().c_str());
        ImGui::DragFloat3("Position", inputVector1, 1.0f);
        ImGui::DragFloat3("Rotation", inputVector2, 1.0f, 0.0f,360.0f);
        ImGui::DragFloat3("Scale", inputVector3, 1.0f, 0.0f);
        if(selected->GetComponent<BoxCollider>()) {
            ImGui::DragFloat3("Colider Size", inputVector4, 1.0f);
            ImGui::DragFloat3("Colider Offset", inputVector5, 1.0f);
        }

        //static char newModelPath[200] = "Write new path here";
        if(selected->GetComponent<Renderer>()){
            ImGui::Text("Path of model: %s", selected->GetComponent<Renderer>()->lastLoadedModelPath.c_str());
            //ImGui::InputText("New model path:",newModelPath,IM_ARRAYSIZE(newModelPath));
            static int selectedModelId = 0;
            std::string stringModelName = modelPaths[selectedModelId].path().filename().string();
            const char * charModelName = stringModelName.c_str();
            if(ImGui::BeginCombo("Models", charModelName))
            {
                for (int n = 0; n < modelPaths.size(); n++)
                {
                    const bool is_selected = (selectedModelId == n);
                    if (ImGui::Selectable(modelPaths[n].path().filename().generic_string().c_str(), is_selected))
                        selectedModelId = n;

                    // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
            if(ImGui::SmallButton("Set new model")){
                std::string path = "Buildings/";
                path += modelPaths[selectedModelId].path().filename().string();
                selected->GetComponent<Renderer>()->LoadModel(path);
            }
        } else {
            ImGui::Text("This object doesnt have a Renderer");
        }
        ImGui::Checkbox("Safety checkbox (check if you want to remove the object)", &safetySwitch);
        if(safetySwitch) {
            if (ImGui::Button("REMOVE")){
                GameObject::Destroy(selected);
                safetySwitch = false;
                displaySelected = false;
                transformExtracted = false;
            }
        }
        if (ImGui::Button("Close"))
        {
            safetySwitch = false;
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
        ImGui::Text("%s", child.second->GetName().c_str());
        ImGui::SameLine();
        if (ImGui::SmallButton(label.c_str())) {
            displaySelected = true;
            safetySwitch = false;
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
    static char inputPath[200] = "";
    static int selectedObjectId = 0;
    ImGui::Begin("Save Menu");
    if (ImGui::SmallButton("Save")) {
        std::filesystem::path path = std::filesystem::current_path();
        path /= "res";
        path /= "ProjectConfig";

        SceneManager::GetInstance()->SaveStaticObjects(path.string(),"map0");
    }
    if (ImGui::SmallButton("Add new default house")){
        SceneManager::GetInstance()->CreatePrefabObject("House");
    }
    //ImGui::InputText("path to new model", inputPath, IM_ARRAYSIZE(inputPath));
    static int selectedModelId = 0;
    std::string stringModelName = modelPaths[selectedModelId].path().filename().string();
    const char * charModelName = stringModelName.c_str();
    if(ImGui::BeginCombo("Models", charModelName))
    {
        for (int n = 0; n < modelPaths.size(); n++)
        {
            const bool is_selected = (selectedModelId == n);
            if (ImGui::Selectable(modelPaths[n].path().filename().generic_string().c_str(), is_selected))
                selectedModelId = n;

            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    if(ImGui::SmallButton("Add house with model at path")){
        //TODO/INFO I assume all house models picked from the picker are in models/buildings
        std::string path = "Buildings/";
        path += modelPaths[selectedModelId].path().filename().string();
        SceneManager::GetInstance()->CreatePrefabObject("House",path);
    }
    if (ImGui::SmallButton("Add new default shop")){
        SceneManager::GetInstance()->CreatePrefabObject("Shop");
    }if (ImGui::SmallButton("Add new default savePoint")){
        SceneManager::GetInstance()->CreatePrefabObject("SavePoint");
    }if (ImGui::SmallButton("Add new Die")) {
        SceneManager::GetInstance()->CreatePrefabObject("Die");
    }
    ImGui::End();
}

void DebugManager::Free() const {

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

std::vector<std::filesystem::directory_entry> DebugManager::FindModelPaths(std::string folderName) {
    std::filesystem::path path = std::filesystem::current_path();
    path /= "res";
    path /= "models";
    path /= "Buildings";
    if(!folderName.empty())
        path /= folderName;
    std::vector<std::filesystem::directory_entry> scannedEntries;
    for(const auto& entry : fs::directory_iterator(path)){
        if(entry.path().string().ends_with(".obj"))
        scannedEntries.push_back(entry);
    }

    return scannedEntries;
}

std::vector<std::filesystem::directory_entry> DebugManager::FindModelFolders() {
    std::filesystem::path path = std::filesystem::current_path();
    path /= "res";
    path /= "models";
    path /= "Buildings";

    std::vector<std::filesystem::directory_entry> scannedEntries;
    for(const auto& entry : fs::directory_iterator(path)){
        if(entry.is_directory())
            scannedEntries.push_back(entry);
    }
    return scannedEntries;
}

#endif
