#include <scigl_render/shader/single_texture_shader.h>

namespace scigl_render
{

Shader SingleTextureShader::create_shader()
{
  return Shader(get_vertex_source(), get_geometry_source(),
                get_fragment_source());
}

std::string SingleTextureShader::get_vertex_source()
{
  return R"(#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal_in;
layout (location = 2) in vec2 texture_coordinate_in;
uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;
uniform vec3 light_position;
out vec2 texture_coordinate;
out vec3 normal;
out vec3 light_direction;

void main()
{
  vec4 local_pos = vec4(position, 1.0);
  vec4 world_pos  =  model_matrix * local_pos;
  vec4 view_pos = view_matrix * world_pos;
  gl_Position = projection_matrix * view_pos;
  normal = normal_in;
  light_direction = normalize(light_position - position);
  texture_coordinate = texture_coordinate_in;
})";
}

std::string SingleTextureShader::get_geometry_source()
{
  return "";
}

std::string SingleTextureShader::get_fragment_source()
{
  return R"(#version 330 core
struct Material
{
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shininess;
};

in vec2 texture_coordinate;
in vec3 normal;
in vec3 light_direction;
out vec4 fragment_color;
uniform Material material;
uniform sampler2D texture0;
uniform float diffuse_texture_strength;
uniform vec3 light_color;

void main()
{
  // ambient
  vec3 ambient = light_color * material.ambient;
  // diffuse
  vec3 diffuse_color =  (material.diffuse, 1) +
    diffuse_texture_strength * texture(texture0, texture_coordinate).xyz;
  vec3 normalized_normal = normalize(normal);
  float diff = max(dot(normalized_normal, light_direction), 0.0);
  vec3 diffuse = light_color * (diff * diffuse_color);
  // specular not used yet        
  vec3 result = ambient + diffuse;
  fragment_color = vec4(result, 1.0);
})";
}
} // namespace scigl_render