#pragma once
#include <scigl_render/render/frame_buffer.hpp>
#include <array>
#include <functional>
#include <memory>

namespace scigl_render
{
/*!
The intedented use case is rendering data offscreen that needs to be processed.
Two pixel buffer objects are use like a double buffer: one PBO is filled
asynchronously with data from the frame buffer while the other one is ready to
be mapped to the clients memory. Consequently the results of a rendering is
delayed by one read call. 
*/
class OffscreenRender
{
public:
  /*!
  Defines a callback when new data has been mapped to the CPU memory.
  Advice: avoid copying the data and use it as readonly memory inside the
  callback to gain the most performance.
  */
  using ProcessDataCallback = std::function<void(const void *data)>;

  /*!
  Renders the scene off-screen and calculates the depth values.
  Throws a runtime_error if the creation failed.
  \param width number of image columns
  \param height number of image rows
  \param pixel_size the size of each pixel: number_channels * sizeof_component
  */
  OffscreenRender(int width, int height, size_t pixel_size,
                  GLenum internal_format);

  /*! Activates the underlying framebuffer for rendering */
  void activate_fbo();

  /*! Clears the contents of the framebuffer */
  void clear_fbo();

  /*! Deactivates the framebuffer. Good practice after finishing the render */
  void deactivate_fbo();

  /*!
  Starts reading from the FBO to the backbuffer PBO.
  \param format specifies the channels, for example GL_RGB
  \param type primitive to store, for example GL_UNSIGNED_BYTE
  */
  void start_read(GLenum format, GLenum type);

  /*!
  Synchronously reads the data from the front buffer (one frame delayed!). After
  finishing the PBOs are swapped, so the next start_render / read_data call will
  use the other PBO. It is important to execute the calculations in the 
  process_data callback to gain performance.
  \param process_data the callback function will be executed if data has been
  read successfully.
  */
  void read_data(const ProcessDataCallback &process_data);

private:
  int width;
  int height;
  // Render to this
  FrameBuffer framebuffer;
  // Two pbos one to read to the other one to map form, alternating
  std::array<GLuint, 2> pbos;
  // transfer from fbo to pbo via glReadPixels
  int pbo_fbo_index = 0;
  // transfer from pbo to CPU memory
  int pbo_map_index = 1;

  /*! Swap the indices for fbo->pbo and mapping */
  void swap_indices();
};
} // namespace scigl_render
