#include <gl3w/GL/gl3w.h>
#include <scigl_render/render/offscreen_render.hpp>
#include <stdexcept>

namespace scigl_render
{
OffscreenRender::OffscreenRender(std::shared_ptr<FrameBuffer> framebuffer,
                                 size_t buffer_size)
    : fbo(framebuffer)
{
  this->buffer_size = buffer_size;
  glGenBuffers(pbos.size(), pbos.data());
  for (size_t i = 0; i < pbos.size(); i++)
  {
    glBindBuffer(GL_PIXEL_PACK_BUFFER, pbos[i]);
    // constantly reading new contents
    glBufferData(GL_PIXEL_PACK_BUFFER, buffer_size, 0, GL_DYNAMIC_READ);
  }
  glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
}

void OffscreenRender::start_render(const std::vector<Model> &models,
                                   const Camera &camera,
                                   const DiffuseLight &light,
                                   const Shader &shader)
{
  // Render the scene
  fbo->clear();
  fbo->activate();
  shader.activate();
  camera.set_in_shader(shader);
  light.set_in_shader(shader);
  for (auto const &model : models)
  {
    model.draw(shader);
  }
  fbo->deactivate();
}

void OffscreenRender::start_read()
{
  fbo->activate();
  glReadBuffer(GL_COLOR_ATTACHMENT0);
  glBindBuffer(GL_PIXEL_PACK_BUFFER, pbos[pbo_fbo_index]);
  glReadPixels(0, 0, fbo->get_width(), fbo->get_height(), fbo->get_format(),
               fbo->get_type(), nullptr);
  glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
  fbo->deactivate();
}

void OffscreenRender::read_data(
    const ProcessDataCallback &process_data)
{
  glBindBuffer(GL_PIXEL_PACK_BUFFER, pbos[pbo_map_index]);
  void *data = glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);
  if (data)
  {
    process_data(data);
  }
  glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
  swap_indices();
}

void OffscreenRender::swap_indices()
{
  int temp = pbo_fbo_index;
  pbo_fbo_index = pbo_map_index;
  pbo_map_index = temp;
}

size_t OffscreenRender::get_buffer_size() const
{
  return buffer_size;
}

} // namespace scigl_render
