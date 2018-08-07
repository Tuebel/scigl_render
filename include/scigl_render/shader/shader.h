#pragma once
#include <string>
#include <gl3w/GL/gl3w.h>
#include <glm/glm.hpp>

namespace scigl_render
{
/*!
Wraps a shader program for simplified compilation and usage.
*/
class Shader
{
public:
  /*!
  Compiles the shader program from the given sources.
  */
  Shader(const std::string &vertex_source, const std::string &geometry_source,
         const std::string &fragment_source);

  /*!
  Activate this shader program.
  */
  void activate() const;

  /*!
  Set the uniform variable value in the shader program.
  \param name the variables name
  \param the value to be set.
  */
  void setBool(const std::string &name, bool value) const;

  /*!
  Set the uniform variable value in the shader program.
  \param name the variables name
  \param the value to be set.
  */
  void setInt(const std::string &name, int value) const;

  /*!
  Set the uniform variable value in the shader program.
  \param name the variables name
  \param the value to be set.
  */
  void setFloat(const std::string &name, float value) const;

  /*!
  Set the uniform variable value in the shader program.
  \param name the variables name
  \param the value to be set.
  */
  void setVec2(const std::string &name, const glm::vec2 &value) const;

  /*!
  Set the uniform variable value in the shader program.
  \param name the variables name
  \param the value to be set.
  */
  void setVec2(const std::string &name, float x, float y) const;

  /*!
  Set the uniform variable value in the shader program.
  \param name the variables name
  \param the value to be set.
  */
  void setVec3(const std::string &name, const glm::vec3 &value) const;

  /*!
  Set the uniform variable value in the shader program.
  \param name the variables name
  \param the value to be set.
  */
  void setVec3(const std::string &name, float x, float y, float z) const;

  /*!
  Set the uniform variable value in the shader program.
  \param name the variables name
  \param the value to be set.
  */
  void setVec4(const std::string &name, const glm::vec4 &value) const;

  /*!
  Set the uniform variable value in the shader program.
  \param name the variables name
  \param the value to be set.
  */
  void setVec4(const std::string &name, float x, float y, float z, float w)
      const;

  /*!
  Set the uniform variable value in the shader program.
  \param name the variables name
  \param the value to be set.
  */
  void setMat2(const std::string &name, const glm::mat2 &mat) const;

  /*!
  Set the uniform variable value in the shader program.
  \param name the variables name
  \param the value to be set.
  */
  void setMat3(const std::string &name, const glm::mat3 &mat) const;

  /*!
  Set the uniform variable value in the shader program.
  \param name the variables name
  \param the value to be set.
  */
  void setMat4(const std::string &name, const glm::mat4 &mat) const;

private:
  /*!
  Reference to this shader program.
  */
  GLuint program_id;

  /*!
  Compiles the shader source code.
  \param source the buffered source code
  \param shader_type for example GL_VERTEX_SHADER & GL_FRAGMENT_SHADER
  \return the reference id of the compiled shader object
  */
  GLuint compile_shader(const std::string &source, GLenum shader_type);

  /*!
  Reads the shader source file into the string. Throws on error.
  \param path the path of the shader source file
  */
  std::string read_shader_file(const std::string &path);

  /*!
  Logs the compilation error info.
  \param shader the reference id of the shader
  \param source the source code of the shader
  */
  void checkShaderCompileErrors(GLuint shader, const std::string &source);

  /*!
  Logs the compilation error info.
  */
  void checkProgramCompileErrors(GLuint program);
};
} // namespace scigl_render
