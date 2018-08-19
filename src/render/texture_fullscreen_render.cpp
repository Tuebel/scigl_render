#include <scigl_render/render/texture_fullscreen_render.hpp>
#include <iostream>

namespace scigl_render
{
/*!
The geometry shader creates the vertices. Nevertheless a vertex shader MUST be
provided so it is simply empty.
*/
const std::string TextureFullscreenRender::vertex_source = R"(
#version 330 core

void main()
{
})";

/*!
Creates a fullscreen quad with z-Coordinate -1.0, so stuff can be rendered in
front of the texture :)
*/
const std::string TextureFullscreenRender::geometry_source = R"(
#version 330 core
layout(points) in;
layout(triangle_strip, max_vertices = 4) out;
out vec2 texture_coordinate;

void main()
{
  gl_Position = vec4(1.0, 1.0, -1.0, 1.0);
  texture_coordinate = vec2(1.0, 1.0);
  EmitVertex();
  gl_Position = vec4(-1.0, 1.0, -1.0, 1.0);
  texture_coordinate = vec2(0.0, 1.0);
  EmitVertex();
  gl_Position = vec4(1.0, -1.0, -1.0, 1.0);
  texture_coordinate = vec2(1.0, 0.0);
  EmitVertex();
  gl_Position = vec4(-1.0, -1.0, -1.0, 1.0);
  texture_coordinate = vec2(0.0, 0.0);
  EmitVertex();
  EndPrimitive();
})";

/*!
Renderesthe texture
*/
const std::string TextureFullscreenRender::fragment_source = R"(
#version 330 core 
in vec2 texture_coordinate;
uniform sampler2D texture0;
out vec4 color;

void main()
{ 
  color = texture(texture0, texture_coordinate);
})";

TextureFullscreenRender::TextureFullscreenRender()
    : shader(vertex_source, geometry_source, fragment_source)
{
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glBindVertexArray(0);
}

void TextureFullscreenRender::draw(GLuint texture)
{
  shader.activate();
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);
  shader.setInt("texture0", 0);
  glBindVertexArray(vao);
  glDrawArrays(GL_POINTS, 0, 1);
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindVertexArray(0);
}
} // namespace scigl_render