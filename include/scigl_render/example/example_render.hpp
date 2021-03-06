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
  \param pixel_size the size of one pixel. Must match the internal format:
  number_of_channels * sizeof(type)
  \param model_path the model to be rendered
  \param light how the light should be parametrized in the scene
  */
  ExampleRender(std::shared_ptr<GLContext> context,
                Shader shader,
                GLenum tex_format, GLenum tex_type, GLint tex_internal_format,
                size_t pixel_size);

  /*!
  Continues to render the next frame.
  */
  void next_frame(const CvCamera &camera, const Model &model,
                  const DiffuseLight &light);

private:
  GLenum texture_format;
  GLenum texture_type;

  std::shared_ptr<GLContext> gl_context;
  // Render targets
  OffscreenRender offscreen_render;
  TextureFullscreenRender texture_render;
  // buffer for reading from the offscreen render
  std::vector<unsigned char> image_buffer;

  // renders the 3D scene
  Shader scene_shader;

  /*!
  Callback for the offscreen render
  */
  void process_data(const void *data);
};
} // namespace scigl_render