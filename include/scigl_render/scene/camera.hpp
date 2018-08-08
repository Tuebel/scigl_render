#pragma once
#include <gl3w/GL/gl3w.h>
#include <scigl_render/shader/shader.hpp>
#include <scigl_render/scene/camera_intrinsics.hpp>
#include <scigl_render/scene/pose.hpp>
#include <vector>

namespace scigl_render
{
/*!
A camera has a position in the world (extrinsics) and projection (intrinsic).
Simplified access to the transformations via translation (x, y, z)
+ yaw, pitch & roll (z, y, x).
*/
class Camera
{
public:
  /*!
  Current pose of the camera in cartesian coordinates.
  */
  QuaternionPose pose;

  CameraIntrinsics intrinsics;

  // Constructor with vectors
  Camera(CameraIntrinsics intrinsics);

  /*!
  The view matrix results by moving the entire scene in the opposite direction
  of the camera transformation.
  */
  glm::mat4 get_view_matrix() const;

  /*!
  The projection matrix based on the intrinsics.
  */
  glm::mat4 get_projection_matrix() const;

  /*!
  Sets the view and projection matrix in the shader, given the current values.
  The variable names must be: "projection_matrix" and "view_matrix"
  */
  void set_in_shader(const Shader &shader) const;

private:
  // Intrinsics don't change, save CPU and calculate once in constructor
  glm::mat4 projection_matrix;
};
} // namespace scigl_render
