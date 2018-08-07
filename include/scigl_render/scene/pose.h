#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace scigl_render
{

/*!
Employs quaternion for orientation instead of euler angles.
*/
struct QuaternionPose
{
  /*! Postion in cartesian coordinates (x,y,z) */
  glm::vec3 position = glm::vec3(0);
  /*! Orientation as quaternion (w,x,y,z)*/
  glm::quat orientation;

  /*!
  Active transformation moves the vertex: v'=T*v
  */
  glm::mat4 active_translation() const
  {
    return glm::translate(glm::mat4(1.0), position);
  }

  /*!
  Active transformation moves the vertex: v'=T*v
  */
  glm::mat4 active_rotation() const
  {
    return glm::mat4_cast(orientation);
  }

  /*!
  Active transformation moves the vertex: v'=T*v
  */
  glm::mat4 active_transformation() const
  {
    // rotate first, translate second
    return active_translation() * active_rotation();
  }

  /*!
  Passive transformation moves the surrounding: v=v'*T*e
  */
  glm::mat4 passive_translation() const
  {
    // the inverse of a translation is walking it backwards
    return glm::translate(glm::mat4(1.0), -1.0f * position);
  }

  /*!
  Passive transformation moves the surrounding: v=v'*T*e
  */
  glm::mat4 passive_rotation() const
  {
    // the inverse of a (orthogonal) rotation matrix is the transposition
    return glm::transpose(active_rotation());
  }

  /*!
  Passive transformation moves the surrounding: v=v'*T*e
  */
  glm::mat4 passive_transformation() const
  {
    // (T*R)^-1 = R^-1 * T^-1
    return passive_rotation() * passive_translation();
  }
};

/*!
A representation that should only be used for testing. 
Basically a integrated converter function for quaternion based pose.
*/
struct CartesianPose
{
  /*! Postion in cartesian coordinates (x,y,z) */
  glm::vec3 position = glm::vec3(0);
  /*! 
  Orientation as Tait-Bryan angles (r_x,r_y,r_z)/(roll,pitch,yaw).
  It is the same as rotating around the fixed axes in the refernce frame.
  */
  glm::vec3 orientation;

  CartesianPose() {}

  CartesianPose(glm::vec3 pos, glm::vec3 ori)
   : position(std::move(pos)), orientation(std::move(ori))
  {
  }

  CartesianPose(QuaternionPose quaterion_pose)
  {
    position = std::move(quaterion_pose.position);
    orientation = glm::eulerAngles(quaterion_pose.orientation);
  }

  QuaternionPose to_quaternion_pose() const
  {
    QuaternionPose pose;
    pose.position = position;
    pose.orientation = glm::quat(orientation);
    return pose;
  }
};
} // namespace scigl_render
