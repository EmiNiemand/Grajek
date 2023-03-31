//
// Created by MasterKtos on 28.03.2023.
//

#ifndef GLOOMENGINE_HIERARCHYMANAGER_H
#define GLOOMENGINE_HIERARCHYMANAGER_H
#include <GloomEngine.h>
#include <GameObjectsAndPrefabs/GameObject.h>


class HierarchyManager {
private:
	static HierarchyManager* hierarchyManager;
	explicit HierarchyManager();

	bool displaySelected;
	std::shared_ptr<GameObject> selected;


	void processChildren(std::shared_ptr<GameObject> gameObject);
	//Function to display RAM and CPU usage.
	void displaySystemInfo();
public:
	HierarchyManager(HierarchyManager& other) = delete;
	void operator=(const HierarchyManager&) = delete;
	virtual ~HierarchyManager();

	static HierarchyManager* GetInstance();

	void Initialize(GLFWwindow* window, const char* glsl_version);
	void Render();
	void Free();

};


#endif //GLOOMENGINE_HIERARCHYMANAGER_H
