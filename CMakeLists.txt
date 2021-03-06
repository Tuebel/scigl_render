cmake_minimum_required(VERSION 3.12)
project(scigl_render)

# Default ROS2 configurations for compilation
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 11)
endif()
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# prefer new ABI for OpenGL
set(OpenGL_GL_PREFERENCE "GLVND")
# find packages
find_package(gl3w REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
find_package(assimp REQUIRED)
find_package(glm REQUIRED)

# library
add_library(scigl_render)
target_sources(scigl_render PRIVATE
  src/gl_context.cpp
  src/render/frame_buffer.cpp
  src/render/offscreen_render.cpp
  src/render/texture_fullscreen_render.cpp
  src/scene/cv_camera.cpp
  src/scene/diffuse_light.cpp
  src/scene/mesh.cpp  
  src/scene/model.cpp
  src/shader/depth_shader.cpp
  src/shader/shader.cpp
  src/shader/single_texture_shader.cpp)
target_include_directories(scigl_render PUBLIC
  ${GLFW_INCLUDE_DIRS}
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
target_link_libraries(scigl_render 
  PUBLIC OpenGL::GL gl3w glm ${GLFW_STATIC_LIBRARIES}
  PRIVATE assimp)

# example apps
add_executable(scigl_viewer
  src/example/scigl_viewer.cpp
  src/example/example_render.cpp)
target_link_libraries(scigl_viewer
  scigl_render)
add_executable(scigl_depth_viewer
  src/example/scigl_depth_viewer.cpp
  src/example/example_render.cpp)
target_link_libraries(scigl_depth_viewer
  scigl_render)
  

# install target
install(DIRECTORY include/scigl_render DESTINATION include)
install(TARGETS scigl_render scigl_viewer scigl_depth_viewer
  EXPORT scigl_render_targets
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES DESTINATION include)
# versioning
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  scigl_renderConfigVersion.cmake
  VERSION 0.1
  COMPATIBILITY AnyNewerVersion)
# for find_package support
install(EXPORT scigl_render_targets
  FILE scigl_render_targets.cmake
  DESTINATION lib/cmake/scigl_render)
configure_file(scigl_renderConfig.cmake.in scigl_renderConfig.cmake @ONLY)
install(FILES 
  "${CMAKE_CURRENT_BINARY_DIR}/scigl_renderConfig.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/scigl_renderConfigVersion.cmake"
  DESTINATION lib/cmake/scigl_render)
# export for source tree
export(TARGETS scigl_render FILE scigl_render_targets.cmake)
export(PACKAGE scigl_render)
