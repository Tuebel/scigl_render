#pragma once
#include <glm/vec3.hpp>

namespace scigl_render
{
struct Material
{
  /*! Refelcted color under ambient ligting.  */
  glm::vec3 ambient;
  /*! Refelcted color under diffuse ligting.  */
  glm::vec3 diffuse;
  /*! Refelcted color under specular ligting.  */
  glm::vec3 specular;
  /*! Scattering of specular light.  */
  float shininess;
};
} // namespace scigl_render