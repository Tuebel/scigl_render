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
  TextureFullscreenRender();

  /*!
  Draws the given texture to a fullscreen quad.
  Won't clear the framebuffer or select a framebuffer.
  Uses its own shader.
  */
  void draw(GLuint texture);

private:
  GLuint vao;
  GLuint vbo;
  const static float vertices[];
  Shader shader;
  static const std::string vertex_source;
  static const std::string geometry_source;
  static const std::string fragment_source;
};
} // namespace scigl_render