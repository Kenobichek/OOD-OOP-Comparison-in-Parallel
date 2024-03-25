cmake_minimum_required(VERSION 3.14)
project(DiplomaProject VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

find_package(Threads REQUIRED)

include(ExternalProject)
ExternalProject_Add(
    ImGui
    PREFIX ${CMAKE_BINARY_DIR}/external
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.82
    TIMEOUT 10
    UPDATE_COMMAND ""
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
)


include_directories(${CMAKE_BINARY_DIR}/external/ImGui/include)
file(GLOB SOURCES "src/*.cc"  "src/*.h")
add_executable(DiplomaProject ${SOURCES})
add_dependencies(DiplomaProject ImGui)
