#include <gl3w/GL/gl3w.h>
#include <scigl_render/gl_context.hpp>
#include <stdexcept>
#include <iostream>

namespace scigl_render
{
GLContext::GLContext(bool visible, bool fullscreen, int w, int h)
    : width(w), height(h)
{
  glfwInit();
  // compability requirements
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  if (!visible)
  {
    glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
  }
  if (fullscreen)
  {
    window = glfwCreateWindow(width, height, "My Title",
                              glfwGetPrimaryMonitor(), NULL);
  }
  else
  {
    window = glfwCreateWindow(width, height, "My Title", NULL, NULL);
  }
  if (!window)
  {
    throw std::runtime_error("Failed to create glfw context");
  }
  glfwMakeContextCurrent(window);
  // load OpenGl core functionaliy
  if (gl3wInit())
  {
    throw std::runtime_error("Failed to initialize OpenGL");
  }
  if (!gl3wIsSupported(4, 2))
  {
    throw std::runtime_error("OpenGL 4.2 not supported");
  }
  std::cout << "OpenGL Version: " << glGetString(GL_VERSION)
            << " GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION)
            << "\n";
  glViewport(0, 0, width, height);
}

GLContext::~GLContext()
{
  glfwDestroyWindow(window);
  glfwTerminate();
  std::cout << "destroyed the contex\n";
}

GLFWwindow *GLContext::get_window()
{
  return window;
}

int GLContext::get_width()
{
  return width;
}

int GLContext::get_height()
{
  return height;
}

} // namespace scigl_render