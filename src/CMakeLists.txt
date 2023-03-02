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
		src/Components/Transform.cpp include/Components/Transform.h src/HighLevelClasses/GameObject.cpp
		include/HighLevelClasses/GameObject.h include/HighLevelClasses/GameObject.h src/Components/Component.cpp include/Components/Component.h include/Components/Component.h include/ProjectSettings.h include/ProjectSettings.h src/GloomEngine.cpp include/GloomEngine.h include/GloomEngine.h src/Components/Renderer.cpp include/Components/Renderer.h include/Components/Renderer.h src/Factories/GameObjectFactory.cpp include/Factiories/GameObjectFactory.h include/Factiories/GameObjectFactory.h src/Factories/ComponentFactory.cpp include/Factiories/ComponentFactory.h include/Factiories/ComponentFactory.h src/Components/Camera.cpp include/Components/Camera.h include/Components/Camera.h)

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

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD 
				   COMMAND ${CMAKE_COMMAND} -E create_symlink 
				   ${CMAKE_SOURCE_DIR}/res 
				   ${CMAKE_CURRENT_BINARY_DIR}/res)

if(MSVC)
    target_compile_definitions(${PROJECT_NAME} PUBLIC NOMINMAX)
endif()
