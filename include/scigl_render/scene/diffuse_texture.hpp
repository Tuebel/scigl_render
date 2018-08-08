#pragma once
#include <gl3w/GL/gl3w.h>
#include <string>

namespace scigl_render
{
/*!
Stores the data of a texture.
*/
struct DiffuseTexture
{
  /*! Where OpenGL stored the texture object */
  GLuint id = 0;
  /*! The strength of the texture overlay */
  float strength;
};
} // namespace scigl_render