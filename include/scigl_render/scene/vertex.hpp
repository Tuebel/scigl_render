#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace scigl_render
{
/*!
Typical cueses that are usually employed in scientific renderings.
*/
struct Vertex
{
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 texture_coordinates;
};
} // namespace scigl_render