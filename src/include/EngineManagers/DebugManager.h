//
// Created by MasterKtos on 28.03.2023.
//
#ifdef DEBUG
#ifndef GLOOMENGINE_DEBUGMANAGER_H
#define GLOOMENGINE_DEBUGMANAGER_H
#include <GloomEngine.h>
#include <GameObjectsAndPrefabs/GameObject.h>
#include <filesystem>

namespace fs = std::filesystem;

class DebugManager {
private:
	inline static DebugManager* hierarchyManager;
	explicit DebugManager();

    //flags to display coresponding menus
    bool displayHierarchyTree;
    bool displaySaveMenu;
    bool displaySystemInfo;


	bool displaySelected;
    bool safetySwitch;
	//flag to check if position of the GameObject was already extracted. 
	bool transformExtracted;
    std::string searchName;
	std::shared_ptr<GameObject> selected;
    std::shared_ptr<GameObject> player;

    int selectedFolderId;
    std::string selectedFolderName;
    std::vector<fs::directory_entry> folderPaths;
    std::vector<fs::directory_entry> modelPaths;

    void MainMenu();
    void HierarchyTree();
	void ProcessChildren(std::shared_ptr<GameObject> gameObject);
	//Conversion from vec3 to float[3] for use in imgui.
	static void ExtractVec3ToFloat3(glm::vec3 input,float* output);
	//Conversion from float[3] to vec3 to set it in the object.
	static glm::vec3 InjectFloat3IntoVec3(float * input);
	//Function to display RAM and CPU usage.
	void DisplaySystemInfo();
	void SaveMenu();

public:
	DebugManager(DebugManager& other) = delete;
	void operator=(const DebugManager&) = delete;
	virtual ~DebugManager();

	static DebugManager* GetInstance();

	void Initialize(GLFWwindow* window, const char* glsl_version);
    void Start();
	void Render();
    void Free() const;

    std::vector<std::filesystem::directory_entry> FindModelFolders();
    std::vector<std::filesystem::directory_entry> FindModelPaths(std::string folderName = "Normal");
};

#endif //GLOOMENGINE_DEBUGMANAGER_H
#endif
