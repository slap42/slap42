cmake_minimum_required(VERSION 3.20)
project(Slap42)

add_executable(${PROJECT_NAME} src/main.cpp)

add_subdirectory(src/extern/glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
