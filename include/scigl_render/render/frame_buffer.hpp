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
  Creates the framebuffer. parametrizes the glTexImage2D method.
  \throws a runtime_error on failure
  \param width of the texture
  \param height of the texture
  \param internal_format number of color components in the texture
  \param format of the pixel data (external)
  \param type data type of the pixel data
  */
  FrameBuffer(int width, int height, GLint internal_format = GL_R32F,
              GLenum format = GL_RED, GLenum type = GL_FLOAT);
  FrameBuffer(const FrameBuffer&) = delete;
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

  /*!
  Returns the texture that the framebuffer renders to.
  This can be used with the texture to quad render to display the framebuffers
  content.
  */
  GLuint get_texture();

  int get_width();

  int get_height();

  GLenum get_format();

  GLenum get_type();

private:
  // Parameterization
  int width, height;
  GLenum format, type;
  // framebuffer object to render the texture into
  GLuint texture_fbo;
  // the texture  attached to the fbo
  GLuint texture;
  // renderbuffer object for the depth testing
  GLuint depth_stencil_rbo;
};
} // namespace scigl_render