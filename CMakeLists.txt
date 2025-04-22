cmake_minimum_required(VERSION 3.30)
project(Project3_DSA)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

# Fetch GLFW automatically
FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG latest
)
FetchContent_MakeAvailable(glfw)

# Fetch OpenGL (on Windows, provided by default)
find_package(OpenGL REQUIRED)

# Auto-clone ImGui
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/external/imgui)
if(NOT EXISTS ${IMGUI_DIR})
    message(STATUS "Cloning ImGui into external/imgui...")
    execute_process(
            COMMAND git clone https://github.com/ocornut/imgui.git ${IMGUI_DIR}
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/external
    )
endif()

# ImGui source files
file(GLOB IMGUI_SRC
        ${IMGUI_DIR}/*.cpp
        ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
        ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

# Create executable
add_executable(Project3_DSA
        src/main.cpp
        src/DataLoader.h
        src/Movie.h
        src/User.h
        src/Graph.h
        src/HashTable.h
        src/DataLoader.cpp
        src/User.cpp
        src/HashTable.cpp
        src/Graph.cpp
        ${IMGUI_SRC}
)

# Include ImGui and backend headers
target_include_directories(Project3_DSA PRIVATE
        ${IMGUI_DIR}
        ${IMGUI_DIR}/backends
)

# Link everything
target_link_libraries(Project3_DSA PRIVATE glfw OpenGL::GL)
