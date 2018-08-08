#include <scigl_render/scene/diffuse_light.hpp>

void scigl_render::DiffuseLight::set_in_shader(const Shader &shader) const
{
  shader.setVec3("light_position", position);
  shader.setVec3("light_color", color);
}