#pragma once
#include <scigl_render/shader/shader.hpp>
#include <scigl_render/scene/mesh.hpp>
#include <scigl_render/scene/pose.hpp>
#include <assimp/scene.h>

namespace scigl_render
{
/*!
Unified class for loading mesh and texture data.
The data is ready to be drawn by OpenGL.
*/
class Model
{
public:
  /*!
  The pose of this object is used for the correct transformation during
  drawing.
  */
  QuaternionPose pose;

  /*!
  Scale the model for example when using different units. Blender uses meters as
  default if the model should be rendered in millimeters set scale to 1000.
  */
  float scale_factor = 1;

  /*!
  Create a model from the given file by importing it via the assimp library.
  Will throw a runtime_error if it fails.
  \param path to the file of the model (*.obj, *.stl, *.blend, .... Full list
  under http://assimp.sourceforge.net/main_features_formats.html)
  */
  Model(const std::string &path);

  /*!
  Draws the model, and thus all its meshes.
  \param shader the shader program used for drawing.
  */
  void draw(const Shader &shader) const;

private:
  std::vector<Mesh> meshes;
  std::string directory;

  /*!
  Processes the nodes recursivly
  \param node the current node to iterate through
  \param scene stores the actual data
  */
  void process_node(const aiNode *node, const aiScene *scene);
};
} // namespace scigl_render
