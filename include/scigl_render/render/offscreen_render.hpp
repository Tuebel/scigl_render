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
transfer and processing. This means the frame beeing read is alway one step
behind the current
*/
class OffscreenRender
{
public:
  /*! Defines a callback when new data has been mapped to the CPU memory */
  using ProcessDataCallback = std::function<void(const void *)>;

  /*!
  Renders the scene off-screen and calculates the depth values.
  Throws a runtime_error if the creation failed.
  \param framebuffer is used to render to. 
  \param buffer_size full sizein bytes of the image to read (width * height * 
  color_channel_count * sizeof(type)). Must match the the framebuffers content.
  */
  OffscreenRender(std::shared_ptr<FrameBuffer> framebuffer, size_t buffer_size);

  /*!
  Starts to render this scene to the framebuffer. This operation is 
  asynchronous.
  \models are rendered in its current pose into one frame. The result of this
  render will be returned after the nex call of this function.
  \param camera the current camera extrinsic and intrinsic parameters
  \param shader the shader that is used to render the scene
  */
  void start_render(const std::vector<Model> &models, const Camera &camera,
                    const DiffuseLight &light, const Shader &shader);

  /*!
  Swaps the PBOs so the one from the last call to start_read can be evaluated by
  read_data. Starts reading from the FBO to the other PBO.
  */
  void start_read();

  /*!
  Synchronously reads the data from the previous start_read call. It is 
  important to execute the calculations in the process_data callback to gain
  performance.
  \param process_data the callback function will be executed if data has been
  read.
  */
  void read_data(const ProcessDataCallback &process_data);

private:
  // Render to this
  std::shared_ptr<FrameBuffer> fbo;
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
