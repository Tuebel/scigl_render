#pragma once
#include <scigl_render/shader/shader.hpp>

namespace scigl_render
{
/*!
This shader renders using a single texture
*/
class SingleTextureShader
{
public:
  static Shader create_shader();

private:
  /*!
  Returns the vertex shader source code.
  */
  static std::string get_vertex_source();
  /*!
  Returns the geometry shader source code.
  */
  static std::string get_geometry_source();
  /*!
  Returns the vertex shader source code.
  */
  static std::string get_fragment_source();
};
} // namespace scigl_render