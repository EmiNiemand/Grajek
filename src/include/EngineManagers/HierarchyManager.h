//
// Created by MasterKtos on 28.03.2023.
//

#ifndef GLOOMENGINE_HIERARCHYMANAGER_H
#define GLOOMENGINE_HIERARCHYMANAGER_H
#include <GloomEngine.h>


class HierarchyManager {
private:
	static HierarchyManager* hierarchyManager;
	explicit HierarchyManager();
public:
	HierarchyManager(HierarchyManager& other) = delete;
	void operator=(const HierarchyManager&) = delete;
	virtual ~HierarchyManager();

	static HierarchyManager* GetInstance();

	void Initialize(GLFWwindow* window, const char* glsl_version);
	void Render();

};


#endif //GLOOMENGINE_HIERARCHYMANAGER_H
