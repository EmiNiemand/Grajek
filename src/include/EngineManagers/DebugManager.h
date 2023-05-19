//
// Created by MasterKtos on 28.03.2023.
//
#ifdef DEBUG
#ifndef GLOOMENGINE_DEBUGMANAGER_H
#define GLOOMENGINE_DEBUGMANAGER_H
#include <GloomEngine.h>
#include <GameObjectsAndPrefabs/GameObject.h>
#include "LowLevelClasses/GameObjectData.h"


class DebugManager {
private:
	inline static DebugManager* hierarchyManager;
	explicit DebugManager();

	bool displaySelected;
	//flag to check if position of the GameObject was already extracted. 
	bool transformExtracted;
	std::shared_ptr<GameObject> selected;
    bool isNewObjectBeingHeld;
    std::shared_ptr<GameObjectData> gameObjectHolder;

	void ProcessChildren(std::shared_ptr<GameObject> gameObject);
	//Conversion from vec3 to float[3] for use in imgui.
	static void ExtractVec3ToFloat3(glm::vec3 input,float* output);
	//Conversion from float[3] to vec3 to set it in the object.
	static glm::vec3 InjectFloat3IntoVec3(float * input);
	//Function to display RAM and CPU usage.
	void DisplaySystemInfo();
	void SaveMenu();
    void ObjectCreator();
public:
	DebugManager(DebugManager& other) = delete;
	void operator=(const DebugManager&) = delete;
	virtual ~DebugManager();

	static DebugManager* GetInstance();

	void Initialize(GLFWwindow* window, const char* glsl_version);
	void Render();
    void Free() const;

    void CreateGameObjectFromData(std::shared_ptr<GameObjectData>);
};

#endif //GLOOMENGINE_DEBUGMANAGER_H
#endif
