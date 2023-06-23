# CPM - package manager
include(CPM)

# OpenGL
find_package(OpenGL REQUIRED)

# glad
set(GLAD_DIR ${CMAKE_CURRENT_LIST_DIR}/glad)
set(glad_SOURCE_DIR ${GLAD_DIR}/include CACHE INTERNAL "")
add_library(glad STATIC ${GLAD_DIR}/src/glad.c)
target_include_directories(glad PRIVATE ${GLAD_DIR}/include)

# stb_image
set(STB_IMAGE_DIR ${CMAKE_CURRENT_LIST_DIR}/stb_image)
set(stb_image_SOURCE_DIR ${STB_IMAGE_DIR} CACHE INTERNAL "")
add_library(stb_image STATIC ${STB_IMAGE_DIR}/stb_image.cpp)
target_include_directories(stb_image PRIVATE ${STB_IMAGE_DIR})

if (CMAKE_BUILD_TYPE MATCHES Debug)
	# imgui
	set(IMGUI_DIR ${CMAKE_CURRENT_LIST_DIR}/imgui)
	set(imgui_SOURCE_DIR ${IMGUI_DIR} CACHE INTERNAL "")
	add_library(imgui STATIC ${IMGUI_DIR}/imgui.cpp
							${IMGUI_DIR}/imgui_demo.cpp
							${IMGUI_DIR}/imgui_draw.cpp
							${IMGUI_DIR}/imgui_tables.cpp
							${IMGUI_DIR}/imgui_widgets.cpp
							${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
							${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
							${IMGUI_DIR}/misc/cpp/imgui_stdlib.cpp)
	target_include_directories(imgui PRIVATE ${IMGUI_DIR})

	set_target_properties(imgui PROPERTIES FOLDER "thirdparty")
endif()

# other
CPMAddPackage("gh:assimp/assimp@5.2.5")
CPMAddPackage("gh:glfw/glfw#3.3.8")
CPMAddPackage("gh:g-truc/glm#0.9.9.8")
CPMAddPackage("gh:gabime/spdlog@1.10.0")
CPMAddPackage("gh:nlohmann/json@3.11.2")
CPMAddPackage("gh:kcat/openal-soft#1.23.1")
CPMAddPackage("gh:aseprite/freetype2#VER-2-10-0")
CPMAddPackage("gh:wolfpld/tracy#v0.9.1")

set_target_properties(glad
                      stb_image 
                      assimp 
                      glfw 
                      glm
                      spdlog
					  nlohmann_json
					  OpenAL
					  freetype
					  TracyClient PROPERTIES FOLDER "thirdparty")

if (TARGET zlibstatic)
    set_target_properties(zlibstatic PROPERTIES FOLDER "thirdparty")
endif()