#include <scigl_render/check_gl_error.hpp>
#include <scigl_render/example/example_render.hpp>
#include <scigl_render/render/offscreen_render.hpp>

namespace scigl_render
{
// I know there is a lot of configuration but it is intendet to be flexible
ExampleRender::ExampleRender(
    std::shared_ptr<GLContext> context,
    Shader shader,
    GLenum tex_format, GLenum tex_type, GLint tex_internal_format,
    size_t pixel_size,
    const std::string &model_path,
    DiffuseLight light)
    : texture_format(tex_format),
      texture_type(tex_type),
      gl_context(context),
      light(std::move(light)),
      scene_shader(std::move(shader)),
      fullscreen_render(gl_context->get_width(),
                        gl_context->get_height(),
                        tex_internal_format)

{
  // Configure the global rendering settings
  glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
  // Offscreen rendering
  framebuffer = std::make_shared<FrameBuffer>(
      gl_context->get_width(), gl_context->get_height(), tex_internal_format,
      tex_format, tex_type);
  buffer_size = gl_context->get_width() * gl_context->get_height() *
                pixel_size;
  offscreen_render = std::make_unique<OffscreenRender>(
      framebuffer, buffer_size);
  // models intialization
  Model model(model_path);
  model.scale_factor = 1;
  models.push_back(model);
  check_gl_error("example render created");
}

void ExampleRender::next_frame(const CvCamera &camera)
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
  std::vector<unsigned char> image(buffer_size);
  memcpy(image.data(), data, buffer_size);
  // draw the copy
  glClearColor(0, 0, 0, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  fullscreen_render.draw(image.data(), texture_format, texture_type);
  // update
  glfwSwapBuffers(gl_context->get_window());
  check_gl_error("process data end");
}
} // namespace scigl_render