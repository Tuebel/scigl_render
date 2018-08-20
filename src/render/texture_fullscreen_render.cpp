#include <scigl_render/render/texture_fullscreen_render.hpp>

namespace scigl_render
{
/*!
The geometry shader creates the vertices. Nevertheless a vertex shader MUST be
provided so it is simply empty.
*/
const std::string vertex_source = R"(
#version 330 core

void main()
{
})";

/*!
Creates a fullscreen quad with z-Coordinate 1.0, so stuff can be rendered in
front of the texture :). Memberberries: looking down z the negative axis.
*/
const std::string geometry_source = R"(
#version 330 core
layout(points) in;
layout(triangle_strip, max_vertices = 4) out;
out vec2 texture_coordinate;

void main()
{
  gl_Position = vec4(1.0, 1.0, 1.0, 1.0);
  texture_coordinate = vec2(1.0, 1.0);
  EmitVertex();
  gl_Position = vec4(-1.0, 1.0, 1.0, 1.0);
  texture_coordinate = vec2(0.0, 1.0);
  EmitVertex();
  gl_Position = vec4(1.0, -1.0, 1.0, 1.0);
  texture_coordinate = vec2(1.0, 0.0);
  EmitVertex();
  gl_Position = vec4(-1.0, -1.0, 1.0, 1.0);
  texture_coordinate = vec2(0.0, 0.0);
  EmitVertex();
  EndPrimitive();
})";

/*!
Renderesthe texture
*/
const std::string fragment_source = R"(
#version 330 core 
in vec2 texture_coordinate;
uniform sampler2D texture0;
out vec4 color;

void main()
{ 
  color = texture(texture0, texture_coordinate);
})";

TextureFullscreenRender::TextureFullscreenRender(
    int width, int height, GLint internal_format)
    : width(width), height(height),
      shader(vertex_source, geometry_source, fragment_source)
{
  // create the texture buffer
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexStorage2D(GL_TEXTURE_2D, 1, internal_format, width, height);
  glBindTexture(GL_TEXTURE_2D, 0);
  // create dummy VAO
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glBindVertexArray(0);
}

void TextureFullscreenRender::draw(
    const GLvoid *pixels, GLenum format, GLenum type)
{
  // With GL_LESS the cleared buffer (z = 1.0) would always win
  glDepthFunc(GL_LEQUAL);
  // activate the shader and texure
  shader.activate();
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);
  // copy data to OpenGL
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, format, type, pixels);
  // draw the image
  shader.setInt("texture0", 0);
  glBindVertexArray(vao);
  glDrawArrays(GL_POINTS, 0, 1);
  // unbind the resources
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindVertexArray(0);
}
} // namespace scigl_render