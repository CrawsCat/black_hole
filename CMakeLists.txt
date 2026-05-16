cmake_minimum_required(VERSION 3.30)

project(cpp_project_name VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# No C++20 modules
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

# Dependencies
find_package(OpenGL REQUIRED)
find_package(GLEW CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(Stb REQUIRED)
find_package(imgui CONFIG REQUIRED)

# Sources
file(GLOB_RECURSE Sim_SOURCES CONFIGURE_DEPENDS 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.inl"
)

# Target
add_executable(cpp_project ${Sim_SOURCES})

# Includes
target_include_directories(cpp_project PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${Stb_INCLUDE_DIR}
)

# Link libraries (NOW it's safe)
target_link_libraries(cpp_project PRIVATE
    GLEW::GLEW
    OpenGL::GL
    glfw
    glm::glm
    glad::glad
    imgui::imgui
)

# Compiler options
if(MSVC)
    target_compile_options(cpp_project PRIVATE
        /W3
        /MP
        #/WX
        /ZI
    )
else()
    target_compile_options(cpp_project PRIVATE 
        -Wall 
        -Wextra 
        -Wpedantic
    )
endif()

# VS niceties
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT cpp_project)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" PREFIX "Source Files" FILES ${Sim_SOURCES})

set_property(GLOBAL PROPERTY USE_FOLDERS ON)