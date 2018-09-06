#pragma once
#include <glm/gtc/matrix_transform.hpp>

namespace scigl_render
{
/*!
Contains all the information to mimic a real camera as virtual camera in OpenGL.
Based on:
http://ksimek.github.io/2013/06/03/calibrated_cameras_in_opengl/

Coordinate convention for the calibration differ, this class uses the OpenCV
camera model:
OpenCV: https://docs.opencv.org/2.4/modules/calib3d/doc/camera_calibration_and_3d_reconstruction.html
Origin right-handed, in upper left corner, x points right, y down.
*/
struct CameraIntrinsics
{
  /*! The width of the image in pixels */
  int width = 640;
  /*! The height of the image in pixels*/
  int height = 480;
  /*! Focal length in x-direction */
  float f_x = 384;
  /*! Focal length in y-direction */
  float f_y = 384;
  /*! Principal point offset in x-direction */
  float c_x = 0;
  /*! Principal point offset in y-direction */
  float c_y = 0;
  /*! Axis skew */
  float s = 0;
  /*! The near plane for OpenGL */
  float near = 0.1;
  /*! The far plane for OpenGL */
  float far = 15;
};
} // namespace scigl_render