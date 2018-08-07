#include <gl3w/GL/gl3w.h>
#include <scigl_render/render/offscreen_render.h>
#include <stdexcept>

namespace scigl_render
{
OffscreenRender::OffscreenRender(std::shared_ptr<FrameBuffer> framebuffer,
                                 size_t buffer_size,
                                 ProcessDataCallback callback)
    : fbo(framebuffer), process_data(callback)
{
  glGenBuffers(pbos.size(), pbos.data());
  for (size_t i = 0; i < pbos.size(); i++)
  {
    glBindBuffer(GL_PIXEL_PACK_BUFFER, pbos[i]);
    glBufferData(GL_PIXEL_PACK_BUFFER, buffer_size, 0, GL_STREAM_READ);
  }
  glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
}

void OffscreenRender::next_frame(const std::vector<Model> &models,
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
  if (!first_frame)
  {
    // Start reading into the fbo->pbo pbo
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    glBindBuffer(GL_PIXEL_PACK_BUFFER, pbos[pbo_fbo_index]);
    glReadPixels(0, 0, fbo->get_width(), fbo->get_height(), fbo->get_format(),
                 fbo->get_type(), nullptr);
    // Map the memory that should be available now and process it!
    // Without processing no performance gain is expected.
    glBindBuffer(GL_PIXEL_PACK_BUFFER, pbos[pbo_map_index]);
    void *data = glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);
    if (data)
    {
      fbo->deactivate();
      process_data(data);
    }
    glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
  }
  else
  {
    first_frame = false;
  }
  swap_indices();
  fbo->deactivate();
  glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
}

void OffscreenRender::swap_indices()
{
  int temp = pbo_fbo_index;
  pbo_fbo_index = pbo_map_index;
  pbo_map_index = temp;
}

} // namespace scigl_render
