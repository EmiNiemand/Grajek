# Add source files
file(GLOB_RECURSE SOURCE_FILES 
	 *.c
	 *.cpp)
	
# Add header files
file(GLOB_RECURSE HEADER_FILES 
	 *.h
	 *.hpp)

# Define the executable
add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES} src/LowLevelClasses/Shader.cpp include/LowLevelClasses/Shader.h src/LowLevelClasses/Mesh.cpp
		include/LowLevelClasses/Mesh.h src/LowLevelClasses/Model.cpp include/LowLevelClasses/Model.h
		src/Components/Transform.cpp include/Components/Transform.h src/GameObjectsAndPrefabs/GameObject.cpp
		include/GameObjectsAndPrefabs/GameObject.h include/GameObjectsAndPrefabs/GameObject.h src/Components/Component.cpp
		include/Components/Component.h include/Components/Component.h include/ProjectSettings.h include/ProjectSettings.h
		src/GloomEngine.cpp include/GloomEngine.h include/GloomEngine.h src/Components/Renderers/Renderer.cpp include/Components/Renderers/Renderer.h
		include/Components/Renderers/Renderer.h src/Factories/GameObjectFactory.cpp include/Factories/GameObjectFactory.h
		include/Factories/GameObjectFactory.h src/Factories/ComponentFactory.cpp include/Factories/ComponentFactory.h
		include/Factories/ComponentFactory.h src/Components/Renderers/Camera.cpp include/Components/Renderers/Camera.h
		include/Components/Renderers/Camera.h src/EngineComponents/EngineRenderer.cpp include/EngineComponents/EngineRenderer.h
		src/Components/Renderers/Lights/PointLight.cpp include/Components/Renderers/Lights/PointLight.h include/Components/Renderers/Lights/PointLight.h
		src/Components/Renderers/Lights/DirectionalLight.cpp include/Components/Renderers/Lights/DirectionalLight.h
		include/Components/Renderers/Lights/DirectionalLight.h src/Components/Renderers/Lights/SpotLight.cpp include/Components/Renderers/Lights/SpotLight.h
		include/Components/Renderers/Lights/SpotLight.h src/Components/PhysicsAndColliders/BoxCollider.cpp include/Components/PhysicsAndColliders/BoxCollider.h
		include/EngineComponents/EngineColliders.h src/EngineComponents/EngineColliders.cpp
		src/EngineComponents/EngineHID.cpp include/EngineComponents/EngineHID.h
		src/Components/PhysicsAndColliders/Rigidbody.cpp include/Components/PhysicsAndColliders/Rigidbody.h src/Components/Scripts/PlayerMovement.cpp include/Components/Scripts/PlayerMovement.h include/Components/Scripts/PlayerMovement.h)

target_compile_definitions(${PROJECT_NAME} PRIVATE GLFW_INCLUDE_NONE)
target_compile_definitions(${PROJECT_NAME} PRIVATE LIBRARY_SUFFIX="")

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
												  ${glad_SOURCE_DIR}
												  ${stb_image_SOURCE_DIR}
												  ${imgui_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})
target_link_libraries(${PROJECT_NAME} glad)
target_link_libraries(${PROJECT_NAME} stb_image)
target_link_libraries(${PROJECT_NAME} assimp)
target_link_libraries(${PROJECT_NAME} glfw)
target_link_libraries(${PROJECT_NAME} imgui)
target_link_libraries(${PROJECT_NAME} spdlog)
target_link_libraries(${PROJECT_NAME} glm::glm)
target_link_libraries(${PROJECT_NAME} nlohmann_json::nlohmann_json)
target_link_libraries(${PROJECT_NAME} OpenAL::OpenAL)
target_link_libraries(${PROJECT_NAME} freetype)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE  "${CMAKE_CXX_FLAGS_RELEASE} -DRELEASE")

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD 
				   COMMAND ${CMAKE_COMMAND} -E create_symlink 
				   ${CMAKE_SOURCE_DIR}/res 
				   ${CMAKE_CURRENT_BINARY_DIR}/res)

if(MSVC)
    target_compile_definitions(${PROJECT_NAME} PUBLIC NOMINMAX)
endif()
