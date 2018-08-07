#pragma once
#include <scigl_render/shader/shader.h>

namespace scigl_render
{
/*!
Returns the source code for shaders that render the absolute depth
of a pixel.
*/
class DepthShader
{
public:
  /*!
  Creates the shader program from the sources.
  */
  static Shader create_shader();

private:
  /*!
  Returns the vertex shader source code.
  */
  static std::string get_vertex_source();
  /*!
  Returns the geometry shader source code. Since this shader is optional,
   returning  an empty string is valid.
  */
  static std::string get_geometry_source();
  /*!
  Returns the vertex shader source code.
  */
  static std::string get_fragment_source();
};
} // namespace scigl_render