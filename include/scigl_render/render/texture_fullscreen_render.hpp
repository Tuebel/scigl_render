#pragma once
#include <gl3w/GL/gl3w.h>
#include <scigl_render/shader/shader.hpp>

namespace scigl_render
{
/*!
Allows rendering a texture to a fullscreen quad.
This class ships its own shader to accomplish the task.
*/
class TextureFullscreenRender
{
public:
  /*!
  Create the renderer with a texture storage with the given format.
  \param internal_format the internal format used in glTexStorage2D
  */
  TextureFullscreenRender(int width, int height,
                          GLint internal_format = GL_RGB8);

  /*! 
  Draws the pixels into the currently selected framebuffer
  \param pixels the pointer to the lower left pixel of the image
  \param format format of a pixel for example GL_RGB
  \param type the type of the storage for example GL_UNSIGNED_BYTE
  */
  void draw(const GLvoid *pixels, GLenum format, GLenum type);

private:
  // store the texture here for rendering
  GLuint texture;
  // properties of the image
  GLsizei width;
  GLsizei height;
  // Dummy VAO since the geometry shader creates the vertices
  GLuint vao;
  Shader shader;
};
} // namespace scigl_render