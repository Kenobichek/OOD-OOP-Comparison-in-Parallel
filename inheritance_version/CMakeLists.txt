cmake_minimum_required (VERSION 3.12)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add source to this project's executable.
file(GLOB files "./src/*.h" "./src/*.cc")

add_executable(inheritance_version ${files})

target_link_libraries(inheritance_version PRIVATE glfw)
target_link_libraries(inheritance_version PUBLIC imgui)
target_link_libraries(inheritance_version PRIVATE glad)
target_link_libraries(inheritance_version PRIVATE TracyClient)
