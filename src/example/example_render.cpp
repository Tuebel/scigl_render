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
    size_t pixel_size)
    : texture_format(tex_format),
      texture_type(tex_type),
      gl_context(context),
      offscreen_render(gl_context->get_width(),
                       gl_context->get_height(),
                       pixel_size,
                       tex_internal_format),
      texture_render(gl_context->get_width(),
                     gl_context->get_height(),
                     tex_internal_format),
      image_buffer(gl_context->get_width() * gl_context->get_height() *
                   pixel_size),
      scene_shader(std::move(shader))
{
  // Configure the global rendering settings
  glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
  check_gl_error("example render created");
}

void ExampleRender::next_frame(const CvCamera &camera, const Model &model,
                               const DiffuseLight &light)
{
  using namespace std::placeholders;
  check_gl_error("next frame begin");
  offscreen_render.clear_fbo();
  offscreen_render.activate_fbo();
  glEnable(GL_DEPTH_TEST);
  camera.set_in_shader(scene_shader);
  light.set_in_shader(scene_shader);
  model.draw(scene_shader);
  offscreen_render.deactivate_fbo();
  offscreen_render.start_read(texture_format, texture_type);
  offscreen_render.read_data(std::bind(&ExampleRender::process_data,
                                       this, _1));
  check_gl_error("next frame end");
}

void ExampleRender::process_data(const void *data)
{
  check_gl_error("process data begin");
  // Test copying the data, buffer_size is in bytes
  memcpy(image_buffer.data(), data, image_buffer.size());
  // draw the copy
  glClearColor(0, 0, 0, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  texture_render.draw(image_buffer.data(), texture_format, texture_type);
  // update
  glfwSwapBuffers(gl_context->get_window());
  check_gl_error("process data end");
}
} // namespace scigl_render