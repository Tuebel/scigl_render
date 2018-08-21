#pragma once
#include <gl3w/GL/gl3w.h>

namespace scigl_render
{
/*!
Wraps a texture frame buffer that can be rendered to offscreen.
Has a color and depth-stencil attachement.
The resources are managed RAII style so the copy constructor is disables; use
a shared_ptr to share the isntance.
*/
class FrameBuffer
{
public:
  /*!
  Creates the framebuffer with a rednerbuffer attachment to render to.
  \throws a runtime_error on failure
  \param width of the texture
  \param height of the texture
  \param internal_format color components & size of each
  */
  FrameBuffer(int width, int height, GLenum internal_format = GL_RGB8);
  FrameBuffer(const FrameBuffer &) = delete;
  ~FrameBuffer();

  /*!
  Use this framebuffer for rendering, sets the color mask and depth testing.
  */
  void activate();

  /*!
  Do not use this framebuffer for rendering anymore.
  */
  void deactivate();

  /*!
  Clears the color and depth values.
  \param color (e.g. absolute depth) using default OpenGL value 0
  \param depth (for depth testing) using default OpenGL value 1
  \param stencil (what to ignore) using OpenGL default value 0
  */
  void clear(float color = 0, float depth = 1, int stencil = 0);

private:
  // Parameterization
  int width, height;
  // framebuffer object to render the texture into
  GLuint fbo;
  // renderbuffer for renderng colors
  GLuint color_rbo;
  // renderbuffer object for the depth testing
  GLuint depth_stencil_rbo;
};
} // namespace scigl_render