#include <scigl_render/scene/camera_intrinsics.h>

namespace scigl_render
{
glm::mat4 CameraIntrinsics::projection_matrix() const
{
  // The idea: http://ksimek.github.io/2013/06/03/calibrated_cameras_in_opengl/
  // perspecitve matrix is almost the intrinisc matrix K
  // only modification: carry on the w-value for z-depth
  glm::mat4 perspective(0);
  perspective[0][0] = f_x;
  perspective[1][0] = s;
  // OpenCV: y points down, OpenGL: y points up
  perspective[1][1] = -f_y;
  perspective[2][0] = -c_x;
  perspective[2][1] = -c_y;
  perspective[2][2] = near + far;
  perspective[2][3] = -1;
  perspective[3][2] = near * far;
  // transform to ndc using the OpenCV left, right, bottom, top
  glm::mat4 ortho = glm::ortho<float>(0, width, height, 0, near, far);
  return (ortho * perspective);
}
} // namespace scigl_render
