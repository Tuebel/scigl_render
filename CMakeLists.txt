cmake_minimum_required(VERSION 3.5)
project(scigl_render)

# Default ROS2 configurations for compilation
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find packages
cmake_policy(SET CMP0072 NEW)
find_package(OpenGL REQUIRED)
find_package(gl3w REQUIRED)
find_package(glfw3 3.2 REQUIRED)
find_package(assimp REQUIRED)
find_package(glm REQUIRED)

# library
add_library(scigl_render
  src/gl_context.cpp
  src/render/frame_buffer.cpp
  src/render/offscreen_render.cpp
  src/render/texture_fullscreen_render.cpp
  src/scene/camera.cpp
  src/scene/diffuse_light.cpp
  src/scene/mesh.cpp  
  src/scene/model.cpp
  src/shader/depth_shader.cpp
  src/shader/single_texture_shader.cpp
  src/shader/shader.cpp)
target_include_directories(scigl_render PUBLIC
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
target_link_libraries(scigl_render PUBLIC
  gl3w
  glfw
  assimp
  glm)

# example app
add_executable(scigl_viewer
  src/example/scigl_viewer.cpp
  src/example/example_render.cpp)
target_link_libraries(scigl_viewer
  scigl_render)

# installation of library
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION include)
install(FILES scigl_render-config.cmake DESTINATION lib/scigl_render)
install(TARGETS scigl_render DESTINATION lib EXPORT scigl_render_targets)
install(EXPORT scigl_render_targets DESTINATION lib/scigl_render)
export(TARGETS scigl_render FILE scigl_render_exports.cmake)
# installation of scigl_viewer
install(TARGETS
  scigl_viewer
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin)
