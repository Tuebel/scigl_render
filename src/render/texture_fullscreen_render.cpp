#include <scigl_render/render/texture_fullscreen_render.h>
#include <iostream>

namespace scigl_render
{
const float TextureFullscreenRender::vertices[] =
    {-1.0f, 1.0f, 0.0f, 1.0f,
     -1.0f, -1.0f, 0.0f, 0.0f,
     1.0f, -1.0f, 1.0f, 0.0f,

     -1.0f, 1.0f, 0.0f, 1.0f,
     1.0f, -1.0f, 1.0f, 0.0f,
     1.0f, 1.0f, 1.0f, 1.0f};

const std::string TextureFullscreenRender::vertex_source =
    R"(#version 330 core
layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texture_coordinate_in;
out vec2 texture_coordinate;

void main()
{
  gl_Position = vec4(position.x, position.y, 0.0, 1.0); 
  texture_coordinate = texture_coordinate_in;
})";

const std::string TextureFullscreenRender::geometry_source = "";

const std::string TextureFullscreenRender::fragment_source =
    R"(#version 330 core 
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
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
  // vertex 3D coordinates
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
  // texture coordinates
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                        (void *)(2 * sizeof(float)));
}

void TextureFullscreenRender::draw(GLuint texture)
{
  shader.activate();
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);
  shader.setInt("texture0", 0);
  glBindVertexArray(vao);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindTexture(GL_TEXTURE_2D, 0);
}
} // namespace scigl_render