#pragma once
#include <scigl_render/render/frame_buffer.hpp>
#include <scigl_render/scene/camera.hpp>
#include <scigl_render/scene/diffuse_light.hpp>
#include <scigl_render/scene/model.hpp>
#include <scigl_render/shader/shader.hpp>
#include <array>
#include <functional>
#include <memory>

namespace scigl_render
{
/*!
The intedented use case is rendering data offscreen that needs to be processed.
The class renders to a framebuffer object and uses PBOs asynchronous data
transfer and processing. This means the performance is only increased if
there is any time intensive processing in the callback.
*/
class OffscreenRender
{
public:
  /*! Defines a callback when new data has been mapped to the CPU memory */
  using ProcessDataCallback = std::function<void(void *)>;

  /*!
  Renders the scene off-screen and calculates the depth values.
  Throws a runtime_error if the creation failed.
  \param framebuffer is used to render to. 
  \param buffer_size full sizein bytes of the image to read (width * height * 
  color_channel_count * sizeof(type)). Must match the the framebuffers content.
  \param callback will be called on next_frame if new data has been mapped
  to the CPU memory. This gives the user the possibility to do calculations or
  copy the data, while the next frame is beeing read.
  */
  OffscreenRender(std::shared_ptr<FrameBuffer> framebuffer, size_t buffer_size,
                  ProcessDataCallback callback);

  /*!
  Continues rendering to the next frame. If data from the previous frame is
  available the ProcessDataCallback is invoked.
  \models are rendered in its current pose into one frame. The result of this
  render will be returned after the nex call of this function.
  \param camera the current camera extrinsic and intrinsic parameters
  \param shader the shader that is used to render the scene
  */
  void next_frame(const std::vector<Model> &models, const Camera &camera,
                  const DiffuseLight &light, const Shader &shader);

private:
  // Render to this
  std::shared_ptr<FrameBuffer> fbo;
  // Two pbos one to read to the other one to map form, alternating
  std::array<GLuint, 2> pbos;
  // transfer from fbo to pbo via glReadPixels
  int pbo_fbo_index = 0;
  // transfer from pbo to CPU memory
  int pbo_map_index = 1;
  // On the first frame there is no previous data available
  bool first_frame = true;
  // Call this if new data has been mapped
  ProcessDataCallback process_data;

  /*! Swap the indices for fbo->pbo and mapping */
  void swap_indices();
};
} // namespace scigl_render
