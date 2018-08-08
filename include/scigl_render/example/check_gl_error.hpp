#pragma once

#include <gl3w/GL/gl3w.h>
#include <sstream>
#include <stdexcept>

namespace scigl_render
{
inline void check_gl_error(const std::string& msg)
{
  GLenum err;
  if ((err = glGetError()) != GL_NO_ERROR)
  {
    std::stringstream stream;
    stream << msg << std::hex << err;
    throw std::runtime_error(stream.str());
  }
}
} // namespace scigl_render