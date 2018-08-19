#include <scigl_render/scene/cv_camera.hpp>

namespace scigl_render
{
CvCamera::CvCamera(CameraIntrinsics intr)
    : projection_matrix(calc_projection_matrix(intr)),
      intrinsics(std::move(intr))

{
  // start in origin
  pose = {};
}

glm::mat4 CvCamera::get_view_matrix() const
{
  auto view_matrix = pose.passive_transformation();
  // Negate y & z axes -> negate corresponding rows (column major mat)
  view_matrix[0][1] = -view_matrix[0][1];
  view_matrix[1][1] = -view_matrix[1][1];
  view_matrix[2][1] = -view_matrix[2][1];
  view_matrix[3][1] = -view_matrix[3][1];

  view_matrix[0][2] = -view_matrix[0][2];
  view_matrix[1][2] = -view_matrix[1][2];
  view_matrix[2][2] = -view_matrix[2][2];
  view_matrix[3][2] = -view_matrix[3][2];

  return view_matrix;
}

glm::mat4 CvCamera::calc_projection_matrix(const CameraIntrinsics &intrinsics)
{
  // The idea: http://ksimek.github.io/2013/06/03/calibrated_cameras_in_opengl/
  // perspecitve matrix is almost the intrinisc matrix K
  // only modification: carry on the w-value for z-depth
  glm::mat4 perspective(0);
  // x-axis is the same for both
  perspective[0][0] = intrinsics.f_x;
  // y-axis points to the opposite direction
  perspective[1][0] = -intrinsics.s;
  perspective[1][1] = -intrinsics.f_y;
  // z-axis points to thhe opposite direction
  perspective[2][0] = -intrinsics.c_x;
  perspective[2][1] = -intrinsics.c_y;
  perspective[2][3] = -1;
  // OpenGL specific: scales w between -near and -far.
  perspective[2][2] = intrinsics.near + intrinsics.far;
  perspective[3][2] = intrinsics.near * intrinsics.far;
  // transform to ndc using the OpenCV left, right, bottom, top
  glm::mat4 ortho = glm::ortho<float>(0, intrinsics.width, intrinsics.height, 0,
                                      intrinsics.near, intrinsics.far);
  return (ortho * perspective);
}

glm::mat4 CvCamera::get_projection_matrix() const
{
  return projection_matrix;
}

void CvCamera::set_in_shader(const Shader &shader) const
{
  shader.activate();
  shader.setMat4("projection_matrix", get_projection_matrix());
  shader.setMat4("view_matrix", get_view_matrix());
}

CameraIntrinsics CvCamera::get_intrinsics() const
{
  return intrinsics;
}
void CvCamera::set_intrinsics(CameraIntrinsics intrinsics)
{
  projection_matrix = calc_projection_matrix(intrinsics);
  this->intrinsics = std::move(intrinsics);
}

} // namespace scigl_render