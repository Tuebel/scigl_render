#include <scigl_render/shader/depth_shader.h>

namespace scigl_render
{
Shader DepthShader::create_shader()
{
  return Shader(get_vertex_source(), get_geometry_source(),
                get_fragment_source());
}

std::string DepthShader::get_vertex_source()
{
  return R"(#version 330 core
layout (location = 0) in vec3 position;
uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;
out float depth;

void main()
{
  vec4 local_pos = vec4(position, 1.0);
  vec4 view_pos  = view_matrix * model_matrix * local_pos;
  depth = view_pos.z;
  // needed for z testing
  gl_Position = projection_matrix * view_pos;
})";
}

std::string DepthShader::get_geometry_source()
{
  return "";
}

std::string DepthShader::get_fragment_source()
{
  return R"(#version 330 core
in float depth;
out float positive_depth;

void main()
{
  // since we are looking down the negative axis
  positive_depth = -depth;
})";
}
} // namespace scigl_render