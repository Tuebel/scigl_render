#pragma once
#include <scigl_render/gl_context.hpp>
#include <scigl_render/render/offscreen_render.hpp>
#include <scigl_render/render/texture_fullscreen_render.hpp>
#include <scigl_render/scene/cv_camera.hpp>
#include <scigl_render/scene/diffuse_light.hpp>
#include <scigl_render/scene/model.hpp>
#include <scigl_render/shader/shader.hpp>
#include <memory>
#include <string>
#include <vector>

namespace scigl_render
{
/*!
Demonstrates how to render a scene with this library. Frankly this is quite
the unnecessary complicated case, since I test rendering offscreen to a FBO,
transfer the texture to a PBO and reading this into CPU memory.Then I push it
back to the GPU as anoter texture generated from the CPU memory and it will is
be displayed on screen.
*/
class ExampleRender
{
public:
  /*!
  Configures the rendering environment and loads the models.
  \param context the environment to configure
  \param shader how to render the scene
  \param tex_format format parameter of glTexImage2D
  \param tex_type type parameter of glTexImage2D
  \param tex_internal_format internal_format parameter of glTexImage2D
  \param model_path the model to be rendered
  \param light how the light should be parametrized in the scene
  */
  ExampleRender(std::shared_ptr<GLContext> context,
                Shader shader, GLenum tex_format, GLenum tex_type,
                GLint tex_internal_format, const std::string &model_path,
                DiffuseLight light);

  /*!
  Continues to render the next frame.
  */
  void next_frame(const CvCamera &camera);

private:
  GLenum texture_format;
  GLenum texture_type;

  std::shared_ptr<GLContext> gl_context;
  DiffuseLight light;
  // renders the 3D scene
  Shader scene_shader;
  // renders a texture to a quad (screen)
  TextureFullscreenRender fullscreen_render;

  std::shared_ptr<FrameBuffer> framebuffer;
  // Size of the image in bytes (width*height*color_channels*sizeof(type))
  size_t buffer_size;
  std::unique_ptr<OffscreenRender> offscreen_render;
  std::vector<Model> models;
  // intermediate texture for rendering to screen after reading pixels
  GLuint quad_texture;

  /*!
  Callback for the offscreen render
  */
  void process_data(const void *data);
};
} // namespace scigl_render