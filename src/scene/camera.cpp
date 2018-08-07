#include <scigl_render/scene/camera.h>

namespace scigl_render
{
Camera::Camera(CameraIntrinsics intr)
    : intrinsics(std::move(intr)),
      projection_matrix(intrinsics.projection_matrix())
{
  // start in origin
  pose = {};
}

glm::mat4 Camera::get_view_matrix() const
{
  return pose.passive_transformation();
}

glm::mat4 Camera::get_projection_matrix() const
{
  return projection_matrix;
}

void Camera::set_in_shader(const Shader &shader) const
{
  shader.setMat4("projection_matrix", get_projection_matrix());
  shader.setMat4("view_matrix", get_view_matrix());
}

} // namespace scigl_render