cmake_minimum_required(VERSION 3.20)
project(Slap42)

set(SLAP42_FILES
  src/slap42/main.cpp
)
source_group("src" FILES ${SLAP42_FILES})

set(SLAP42_GRAPHICS_FILES
  src/slap42/graphics/camera.hpp
  src/slap42/graphics/camera.cpp
  src/slap42/graphics/texture.hpp
  src/slap42/graphics/texture.cpp
  src/slap42/graphics/texture_array.hpp
  src/slap42/graphics/texture_array.cpp
)
source_group("src/graphics" FILES ${SLAP42_GRAPHICS_FILES})

set(SLAP42_GRAPHICS_MESHES_FILES
  src/slap42/graphics/meshes/mesh.hpp
  src/slap42/graphics/meshes/mesh.cpp
  src/slap42/graphics/meshes/entity_mesh.hpp
  src/slap42/graphics/meshes/entity_mesh.cpp
  src/slap42/graphics/meshes/terrain_mesh.hpp
  src/slap42/graphics/meshes/terrain_mesh.cpp
)
source_group("src/graphics/meshes" FILES ${SLAP42_GRAPHICS_MESHES_FILES})

set(SLAP42_GRAPHICS_SHADERS_FILES
  src/slap42/graphics/shaders/shader.hpp
  src/slap42/graphics/shaders/shader.cpp
  src/slap42/graphics/shaders/entity_shader.hpp
  src/slap42/graphics/shaders/entity_shader.cpp
  src/slap42/graphics/shaders/terrain_shader.hpp
  src/slap42/graphics/shaders/terrain_shader.cpp
)
source_group("src/graphics/shaders" FILES ${SLAP42_GRAPHICS_SHADERS_FILES})

set(SLAP42_LEVEL_FILES
  src/slap42/level/chunk.hpp
  src/slap42/level/chunk.cpp
  src/slap42/level/level.hpp
  src/slap42/level/level.cpp
)
source_group("src/level" FILES ${SLAP42_LEVEL_FILES})

set(SLAP42_NETWORKING_FILES
  src/slap42/networking/message_types.hpp
  src/slap42/networking/message_serializer.hpp
  src/slap42/networking/server.hpp
  src/slap42/networking/server.cpp
  src/slap42/networking/client.hpp
  src/slap42/networking/client.cpp
  src/slap42/networking/peer_data.hpp
)
source_group("src/networking" FILES ${SLAP42_NETWORKING_FILES})

set(SLAP42_MENUS_FILES
  src/slap42/menus/menu_state.hpp
  src/slap42/menus/menu_state_machine.hpp
  src/slap42/menus/menu_state_machine.cpp
  src/slap42/menus/host_join_menu.hpp
  src/slap42/menus/host_join_menu.cpp
  src/slap42/menus/join_menu.hpp
  src/slap42/menus/join_menu.cpp
  src/slap42/menus/join_error_menu.hpp
  src/slap42/menus/join_error_menu.cpp
)
source_group("src/menus" FILES ${SLAP42_MENUS_FILES})

set(SLAP42_HUD_PANELS_FILES
  src/slap42/hud_panels/chat_panel.hpp
  src/slap42/hud_panels/chat_panel.cpp
  src/slap42/hud_panels/server_panel.hpp
  src/slap42/hud_panels/server_panel.cpp
)
source_group("src/hud_panels" FILES ${SLAP42_HUD_PANELS_FILES})

set(SLAP42_UTILS_FILES
  src/slap42/utils/hash.hpp
)
source_group("src/utils" FILES ${SLAP42_UTILS_FILES})

set(SLAP42_WINDOW_FILES
  src/slap42/window/controls.hpp
  src/slap42/window/controls.cpp
  src/slap42/window/window.hpp
  src/slap42/window/window.cpp
)
source_group("src/window" FILES ${SLAP42_WINDOW_FILES})

add_executable(${PROJECT_NAME}
  ${SLAP42_FILES}
  ${SLAP42_GRAPHICS_FILES}
  ${SLAP42_GRAPHICS_MESHES_FILES}
  ${SLAP42_GRAPHICS_SHADERS_FILES}
  ${SLAP42_LEVEL_FILES}
  ${SLAP42_NETWORKING_FILES}
  ${SLAP42_MENUS_FILES}
  ${SLAP42_HUD_PANELS_FILES}
  ${SLAP42_UTILS_FILES}
  ${SLAP42_WINDOW_FILES}
)

target_include_directories(${PROJECT_NAME} PRIVATE src/slap42)

if(APPLE)
  set_target_properties(${PROJECT_NAME} PROPERTIES XCODE_GENERATE_SCHEME TRUE XCODE_SCHEME_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endif()
if(WIN32)
  set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endif()

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
if(WIN32)
  set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE $<CONFIG:Release>)
endif()

add_subdirectory(src/extern/bytepack)
target_link_libraries(${PROJECT_NAME} PRIVATE bytepack)

add_subdirectory(src/extern/enet)
target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE src/extern/enet/include)
if(WIN32)
  target_compile_definitions(enet PRIVATE _WINSOCK_DEPRECATED_NO_WARNINGS)
endif()
target_link_libraries(${PROJECT_NAME} PRIVATE enet)

add_subdirectory(src/extern/glad)
target_link_libraries(${PROJECT_NAME} PRIVATE glad)

add_subdirectory(src/extern/glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)

add_subdirectory(src/extern/glm)
target_link_libraries(${PROJECT_NAME} PRIVATE glm)

add_subdirectory(src/extern/imgui)
target_link_libraries(${PROJECT_NAME} PRIVATE imgui)

add_subdirectory(src/extern/opensimplex)
target_link_libraries(${PROJECT_NAME} PRIVATE opensimplex)

add_subdirectory(src/extern/stb)
target_link_libraries(${PROJECT_NAME} PRIVATE stb)
