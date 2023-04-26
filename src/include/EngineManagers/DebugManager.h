//
// Created by MasterKtos on 28.03.2023.
//

#ifndef GLOOMENGINE_DEBUGMANAGER_H
#define GLOOMENGINE_DEBUGMANAGER_H
#include <GloomEngine.h>
#include <GameObjectsAndPrefabs/GameObject.h>


class DebugManager {
private:
	inline static DebugManager* hierarchyManager;
	explicit DebugManager();

	bool displaySelected;
	//flag to check if position of the GameObject was already extracted. 
	bool transformExtracted;
	std::shared_ptr<GameObject> selected;


	void ProcessChildren(std::shared_ptr<GameObject> gameObject);
	//Conversion from vec3 to float[3] for use in imgui.
	void ExtractVec3ToFloat3(glm::vec3 input,float* output);
	//Conversion from float[3] to vec3 to set it in the object.
	glm::vec3 InjectFloat3IntoVec3(float * input);
	//Function to display RAM and CPU usage.
	void DisplaySystemInfo();
	void SaveMenu();
public:
	DebugManager(DebugManager& other) = delete;
	void operator=(const DebugManager&) = delete;
	virtual ~DebugManager();

	static DebugManager* GetInstance();

	void Initialize(GLFWwindow* window, const char* glsl_version);
	void Render();
    void Free() const;
};


#endif //GLOOMENGINE_DEBUGMANAGER_H
