cmake_minimum_required(VERSION 3.20)
project(Slap42)

set(SLAP42_FILES
  src/slap42/main.cpp
)
source_group("src" FILES ${SLAP42_FILES})

set(SLAP42_GRAPHICS_FILES
  src/slap42/graphics/mesh.hpp
  src/slap42/graphics/mesh.cpp
  src/slap42/graphics/shader.hpp
  src/slap42/graphics/shader.cpp
  src/slap42/graphics/camera.hpp
  src/slap42/graphics/camera.cpp
)
source_group("src/graphics" FILES ${SLAP42_GRAPHICS_FILES})

add_executable(${PROJECT_NAME}
  ${SLAP42_FILES}
  ${SLAP42_GRAPHICS_FILES}
)

add_subdirectory(src/extern/glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)

add_subdirectory(src/extern/glad)
target_link_libraries(${PROJECT_NAME} PRIVATE glad)

add_subdirectory(src/extern/glm)
target_link_libraries(${PROJECT_NAME} PRIVATE glm)
