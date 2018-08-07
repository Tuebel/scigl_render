#include <scigl_render/shader/shader.h>
#include <stdexcept>

namespace scigl_render
{
Shader::Shader(const std::string &vertex_source, const std::string &geometry_source,
               const std::string &fragment_source)
{
  GLuint vertex_shader = compile_shader(vertex_source, GL_VERTEX_SHADER);
  GLuint geometry_shader = compile_shader(geometry_source, GL_GEOMETRY_SHADER);
  GLuint fragment_shader = compile_shader(fragment_source, GL_FRAGMENT_SHADER);
  // shader program
  program_id = glCreateProgram();
  glAttachShader(program_id, vertex_shader);
  if (geometry_shader != 0)
  {
    glAttachShader(program_id, geometry_shader);
  }
  glAttachShader(program_id, fragment_shader);
  glLinkProgram(program_id);
  checkProgramCompileErrors(program_id);
  // Delete unused resources
  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);
}

GLuint Shader::compile_shader(const std::string &source, GLenum shader_type)
{
  if (!source.empty())
  {
    GLuint shader_id = glCreateShader(shader_type);
    // Cast string to const GLchar* since OpenGL expects const GLchar**
    const GLchar *gl_source = source.c_str();
    glShaderSource(shader_id, 1, &gl_source, NULL);
    glCompileShader(shader_id);
    checkShaderCompileErrors(shader_id, source);
    return shader_id;
  }
  else
  {
    return 0;
  }
}

void Shader::checkShaderCompileErrors(GLuint shader, const std::string &source)
{
  GLint success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    GLchar infoLog[1024];
    glGetShaderInfoLog(shader, 1024, NULL, infoLog);
    throw std::runtime_error("Shader compilation error, shader type: "
                             "\nShaderInfoLog: " +
                             std::string(infoLog) +
                             "\nShader source:\n" + source);
  }
}

void Shader::checkProgramCompileErrors(GLuint program)
{
  GLint success;
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success)
  {
    GLchar infoLog[1024];
    glGetProgramInfoLog(program, 1024, NULL, infoLog);
    throw std::runtime_error("Program linking error\nInfoLog: " +
                             std::string(infoLog));
  }
}

void Shader::activate() const
{
  glUseProgram(program_id);
}

void Shader::setBool(const std::string &name, bool value) const
{
  glUniform1i(glGetUniformLocation(program_id, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const
{
  glUniform1i(glGetUniformLocation(program_id, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const
{
  glUniform1f(glGetUniformLocation(program_id, name.c_str()), value);
}

void Shader::setVec2(const std::string &name, const glm::vec2 &value) const
{
  glUniform2fv(glGetUniformLocation(program_id, name.c_str()), 1, &value[0]);
}

void Shader::setVec2(const std::string &name, float x, float y) const
{
  glUniform2f(glGetUniformLocation(program_id, name.c_str()), x, y);
}

void Shader::setVec3(const std::string &name, const glm::vec3 &value) const
{
  glUniform3fv(glGetUniformLocation(program_id, name.c_str()), 1, &value[0]);
}

void Shader::setVec3(const std::string &name, float x, float y, float z) const
{
  glUniform3f(glGetUniformLocation(program_id, name.c_str()), x, y, z);
}

void Shader::setVec4(const std::string &name, const glm::vec4 &value) const
{
  glUniform4fv(glGetUniformLocation(program_id, name.c_str()), 1, &value[0]);
}

void Shader::setVec4(const std::string &name, float x, float y, float z,
                     float w) const
{
  glUniform4f(glGetUniformLocation(program_id, name.c_str()), x, y, z, w);
}

void Shader::setMat2(const std::string &name, const glm::mat2 &mat) const
{
  glUniformMatrix2fv(glGetUniformLocation(program_id, name.c_str()), 1,
                     GL_FALSE, &mat[0][0]);
}

void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const
{
  glUniformMatrix3fv(glGetUniformLocation(program_id, name.c_str()), 1,
                     GL_FALSE, &mat[0][0]);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
{
  glUniformMatrix4fv(glGetUniformLocation(program_id, name.c_str()), 1,
                     GL_FALSE, &mat[0][0]);
}

} // namespace scigl_render
