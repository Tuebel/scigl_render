#include <scigl_render/scene/camera.hpp>

namespace scigl_render
{
Camera::Camera(CameraIntrinsics intr)
    : projection_matrix(calc_projection_matrix(intr)),
      intrinsics(std::move(intr))

{
  // start in origin
  pose = {};
}

glm::mat4 Camera::get_view_matrix() const
{
  return pose.passive_transformation();
}

glm::mat4 Camera::calc_projection_matrix(const CameraIntrinsics &intrinsics)
{
  // The idea: http://ksimek.github.io/2013/06/03/calibrated_cameras_in_opengl/
  // perspecitve matrix is almost the intrinisc matrix K
  // only modification: carry on the w-value for z-depth
  glm::mat4 perspective(0);
  perspective[0][0] = intrinsics.f_x;
  perspective[1][0] = intrinsics.s;
  // OpenCV: y points down, OpenGL: y points up
  perspective[1][1] = -intrinsics.f_y;
  perspective[2][0] = -intrinsics.c_x;
  perspective[2][1] = -intrinsics.c_y;
  perspective[2][2] = intrinsics.near + intrinsics.far;
  perspective[2][3] = -1;
  perspective[3][2] = intrinsics.near * intrinsics.far;
  // transform to ndc using the OpenCV left, right, bottom, top
  glm::mat4 ortho = glm::ortho<float>(0, intrinsics.width, intrinsics.height, 0,
                                      intrinsics.near, intrinsics.far);
  return (ortho * perspective);
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

CameraIntrinsics Camera::get_intrinsics() const
{
  return intrinsics;
}
void Camera::set_intrinsics(CameraIntrinsics intrinsics)
{
  projection_matrix = calc_projection_matrix(intrinsics);
  this->intrinsics = std::move(intrinsics);
}

} // namespace scigl_render