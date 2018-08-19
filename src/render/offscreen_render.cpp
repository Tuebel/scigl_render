#include <gl3w/GL/gl3w.h>
#include <scigl_render/render/offscreen_render.hpp>
#include <stdexcept>

namespace scigl_render
{
OffscreenRender::OffscreenRender(int width, int height, size_t pixel_size,
                                 GLenum internal_format)
    : width(width), height(height),
      framebuffer(width, height, internal_format)
{
  glGenBuffers(pbos.size(), pbos.data());
  for (size_t i = 0; i < pbos.size(); i++)
  {
    glBindBuffer(GL_PIXEL_PACK_BUFFER, pbos[i]);
    // constantly reading new contents
    glBufferData(GL_PIXEL_PACK_BUFFER, width * height * pixel_size, 0,
                 GL_DYNAMIC_READ);
  }
  glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
}

void OffscreenRender::activate_fbo()
{
  framebuffer.activate();
}

void OffscreenRender::clear_fbo()
{
  framebuffer.clear();
}

void OffscreenRender::deactivate_fbo()
{
  framebuffer.deactivate();
}

void OffscreenRender::start_read(GLenum format, GLenum type)
{
  framebuffer.activate();
  glReadBuffer(GL_COLOR_ATTACHMENT0);
  glBindBuffer(GL_PIXEL_PACK_BUFFER, pbos[pbo_fbo_index]);
  glReadPixels(0, 0, width, height, format, type, nullptr);
  glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
  framebuffer.deactivate();
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

} // namespace scigl_render
