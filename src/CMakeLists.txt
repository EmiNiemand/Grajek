# Add source files
file(GLOB_RECURSE SOURCE_FILES 
	 *.c
	 *.cpp)
	
# Add header files
file(GLOB_RECURSE HEADER_FILES 
	 *.h
	 *.hpp)

IF (WIN32)
	enable_language("RC")
	set (WIN32_RESOURCES ${CMAKE_SOURCE_DIR}/res/gameResources.rc)
ENDIF()

# Define the executable
add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES} ${WIN32_RESOURCES})

target_compile_definitions(${PROJECT_NAME} PRIVATE GLFW_INCLUDE_NONE)
target_compile_definitions(${PROJECT_NAME} PRIVATE LIBRARY_SUFFIX="")

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
												  ${glad_SOURCE_DIR}
												  ${stb_image_SOURCE_DIR})

if (CMAKE_BUILD_TYPE MATCHES Debug)
	target_include_directories(${PROJECT_NAME} PUBLIC ${imgui_SOURCE_DIR})
	target_link_libraries(${PROJECT_NAME} imgui)
endif()

target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})
target_link_libraries(${PROJECT_NAME} glad)
target_link_libraries(${PROJECT_NAME} stb_image)
target_link_libraries(${PROJECT_NAME} assimp)
target_link_libraries(${PROJECT_NAME} glfw)
target_link_libraries(${PROJECT_NAME} spdlog)
target_link_libraries(${PROJECT_NAME} glm::glm)
target_link_libraries(${PROJECT_NAME} nlohmann_json::nlohmann_json)
target_link_libraries(${PROJECT_NAME} OpenAL::OpenAL)
target_link_libraries(${PROJECT_NAME} freetype)
target_link_libraries(${PROJECT_NAME} TracyClient)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DRELEASE")

set_target_properties(OpenAL
		PROPERTIES
		LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

set_target_properties(OpenAL
		PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
				   COMMAND ${CMAKE_COMMAND} -E create_symlink
				   ${CMAKE_SOURCE_DIR}/res 
				   ${CMAKE_CURRENT_BINARY_DIR}/res)

if(MSVC)
    target_compile_definitions(${PROJECT_NAME} PUBLIC NOMINMAX)
endif()
