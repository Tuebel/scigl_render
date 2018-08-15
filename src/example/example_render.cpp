#include <scigl_render/check_gl_error.hpp>
#include <scigl_render/example/example_render.hpp>
#include <scigl_render/render/offscreen_render.hpp>

namespace scigl_render
{
// const GLenum ExampleRender::FORMAT = GL_RGBA;
// const GLenum ExampleRender::TYPE = GL_UNSIGNED_BYTE;
// const GLint ExampleRender::INTERNAL_FORMAT = GL_RGBA8;

// I know there is a lot of configuration but it is intendet to be flexible
ExampleRender::ExampleRender(std::shared_ptr<GLContext> context,
                             Shader shader, GLenum tex_format, GLenum tex_type,
                             GLint tex_internal_format,
                             const std::string &model_path, DiffuseLight light)
    : texture_format(tex_format), texture_type(tex_type), gl_context(context),
      light(std::move(light)), scene_shader(std::move(shader))
{
  // Configure the global rendering settings
  glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
  // Offscreen render context
  framebuffer = std::make_shared<FrameBuffer>(
      gl_context->get_width(), gl_context->get_height(), tex_internal_format,
      tex_format, tex_type);
  // Since I don't know how to extract sizes from the GLenum type, just reserve
  // plenty for RGBA32F
  buffer_size = gl_context->get_width() * gl_context->get_height() *
                4 * sizeof(float);
  offscreen_render = std::make_unique<OffscreenRender>(
      framebuffer, buffer_size);
  // models are expected as vector
  Model model(model_path);
  model.scale_factor = 1;
  models.push_back(model);
  // Create intermediate texture to render on screen after reading pixels
  glGenTextures(1, &quad_texture);
  glBindTexture(GL_TEXTURE_2D, quad_texture);
  glTexStorage2D(GL_TEXTURE_2D, 1, tex_internal_format, gl_context->get_width(),
                 gl_context->get_height());
  glBindTexture(GL_TEXTURE_2D, 0);
  check_gl_error("example render created");
}

void ExampleRender::next_frame(const Camera &camera)
{
  using namespace std::placeholders;
  check_gl_error("next frame begin");
  glEnable(GL_DEPTH_TEST);
  offscreen_render->start_render(models, camera, light, scene_shader);
  offscreen_render->start_read();
  offscreen_render->read_data(std::bind(&ExampleRender::process_data,
                                        this, _1));
  check_gl_error("next frame end");
}

void ExampleRender::process_data(const void *data)
{
  check_gl_error("process data begin");
  // Now draw framebuffer to the default window framebuffer
  framebuffer->deactivate();
  // Test copying the data, buffer_size is in bytes
  std::vector<float> image(buffer_size / sizeof(float));
  memcpy(image.data(), data, buffer_size);
  // Store the copy in the texture
  glBindTexture(GL_TEXTURE_2D, quad_texture);
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, gl_context->get_width(),
                  gl_context->get_height(), texture_format, texture_type,
                  image.data());
  // delete[] test;
  glDisable(GL_DEPTH_TEST);
  glClearColor(0, 0, 0, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glActiveTexture(GL_TEXTURE0);
  fullscreen_render.draw(framebuffer->get_texture());
  glBindTexture(GL_TEXTURE_2D, 0);
  // update
  glfwSwapBuffers(gl_context->get_window());
  check_gl_error("process data end");
}
} // namespace scigl_render