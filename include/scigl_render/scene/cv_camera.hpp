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
The extrinsics and intrinsics are calculated using the OpenCV camera model:
  - x-axis points right
  - y-axis points down
  - z-axis points into the image plane
So use the OpenCV / ROS frame convention to set the extrinsic pose.
*/
class CvCamera
{
public:
  /*!
  Current pose of the camera in cartesian coordinates according the ROS camera
  frame convention (see Header defintion: 
  http://docs.ros.org/melodic/api/sensor_msgs/html/msg/CameraInfo.html)
  */
  QuaternionPose pose;

  /*!
  Default constructor. Don't forget to set the intrinsics!
  */
  CvCamera();
  /*!
  Constructor with intrinsics
  */
  CvCamera(CameraIntrinsics intrinsics);

  /*!
  The view matrix results by moving the entire scene in the opposite direction
  of the camera transformation (passive transformation of the camera). Converts
  the OpenCV/ROS extrinsic pose to the OpenGL pose (negate y & z axes).
  */
  glm::mat4 get_view_matrix() const;

  /*!
  The projection matrix based on the intrinsics configured on construction.
  */
  glm::mat4 get_projection_matrix() const;

  /*!
  Sets the view and projection matrix in the shader, given the current values.
  The variable names must be: "projection_matrix" and "view_matrix"
  */
  void set_in_shader(const Shader &shader) const;

  CameraIntrinsics get_intrinsics() const;
  void set_intrinsics(CameraIntrinsics intrinsics);

private:
  glm::mat4 projection_matrix;
  CameraIntrinsics intrinsics;

  /*!
  Get the projection matrix for the given camera intrinsics.
  \return a projection matrix calculated that transforms from view to clipping
  space
  */
  static glm::mat4 calc_projection_matrix(const CameraIntrinsics &intrinsics);
};
} // namespace scigl_render
