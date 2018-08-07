cmake_minimum_required(VERSION 3.5)
project(scigl_render)

# Default ROS2 configurations for compilation
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# OpenGL
cmake_policy(SET CMP0072 NEW)
find_package(OpenGL REQUIRED)
# OpenGL function loader
find_package(gl3w REQUIRED)
# OpenGL context provider
find_package(glfw3 3.2 REQUIRED)
# Asset loading
find_package(assimp REQUIRED)
# OpenGL math
find_package(glm REQUIRED)

# Include headers
include_directories(include)

# Source files for libray
set(LIB_SRCS
  src/gl_context.cpp
  src/render/frame_buffer.cpp
  src/render/offscreen_render.cpp
  src/render/texture_fullscreen_render.cpp
  src/scene/camera_intrinsics.cpp
  src/scene/camera.cpp
  src/scene/diffuse_light.cpp
  src/scene/mesh.cpp  
  src/scene/model.cpp
  src/shader/depth_shader.cpp
  src/shader/single_texture_shader.cpp
  src/shader/shader.cpp
)

set(EXAMPLE_SRCS
  src/example/example_render.cpp
)

# Build library
add_library(scigl_render
  ${LIB_SRCS})
target_link_libraries(scigl_render
  gl3w
  glfw
  assimp
  glm)

# Example application
add_library(scigl_render_example
  ${EXAMPLE_SRCS})
target_link_libraries(scigl_render_example
  scigl_render)
add_executable(viewer_app
  src/example/viewer_app.cpp)
target_link_libraries(viewer_app
  scigl_render_example)

## Install include dir
install(
  DIRECTORY include/scigl_render
  DESTINATION include)

## Install executable and lib
install(TARGETS
  scigl_render
  viewer_app
  ARCHIVE DESTINATION lib/${PROJECT_NAME}
  LIBRARY DESTINATION lib/${PROJECT_NAME}
  RUNTIME DESTINATION lib/${PROJECT_NAME})
