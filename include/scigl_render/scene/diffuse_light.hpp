#pragma once
#include <scigl_render/shader/shader.hpp>
#include <glm/vec3.hpp>

namespace scigl_render
{
/*!
A diffuse ligth source.
*/
struct DiffuseLight
{
  /*!Cartesian world position of the ligth */
  glm::vec3 position;
  /*!RGB color of the light, range from 0 to 1.*/
  glm::vec3 color;
  /*!
  Sets the current light in the shader which must provide:
  uniform vec3 light_position;
  uniform vec3 light_color;
  */
  void set_in_shader(const Shader& shader) const;
};
} // namespace scigl_render
