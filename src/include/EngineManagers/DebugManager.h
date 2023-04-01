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
	std::shared_ptr<GameObject> selected;


	void processChildren(std::shared_ptr<GameObject> gameObject);
	void DisplaySystemInfo();
	//Function to display RAM and CPU usage.
public:
	DebugManager(DebugManager& other) = delete;
	void operator=(const DebugManager&) = delete;
	virtual ~DebugManager();

	static DebugManager* GetInstance();

	void Initialize(GLFWwindow* window, const char* glsl_version);
	void Render();
	void Free();

};


#endif //GLOOMENGINE_DEBUGMANAGER_H
