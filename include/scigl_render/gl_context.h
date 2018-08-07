#pragma once
#include <GLFW/glfw3.h>

namespace scigl_render
{
/*!
Bundles the creation and destruction of the GLFW render context RAII style.
The copy constructor is deleted, use a shard_ptr.
*/
class GLContext
{
public:
  GLContext(bool visible, int width, int height);
  GLContext(const GLContext&) = delete;
  ~GLContext();

  /*!
  \return the underlying window.
  */
  GLFWwindow *get_window();

  int get_width();
  int get_height();

private:
  GLFWwindow *window;
  int width, height;
};
} // namespace scigl_render