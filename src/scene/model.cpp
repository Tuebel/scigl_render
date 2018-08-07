#include <scigl_render/scene/model.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace scigl_render
{
Model::Model(const std::string &path)
    : directory(path.substr(0, path.find_last_of('/')))
{
  pose = {};
  Assimp::Importer importer;
  // Read from file with post processing:
  // Realtime (some of them are transforming faces to triangles, creating
  // normals, optimizing the meshes, ...). Loading time is not as important as
  // fast rendering. The models will probably be quite small.
  // Flip the texture (not upside down anymore)
  const aiScene *scene = importer.ReadFile(
      path, aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_FlipUVs);
  std::cout << "read scene\n";
  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
  {
    throw std::runtime_error("error loading scene " +
                             std::string(importer.GetErrorString()));
  }
  std::cout << "process node\n";
  // process ASSIMP's root node recursively
  process_node(scene->mRootNode, scene);
}

void Model::process_node(const aiNode *node, const aiScene *scene)
{
  for (unsigned int i = 0; i < node->mNumMeshes; i++)
  {
    aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
    meshes.push_back(Mesh(mesh, scene));
  }
  for (unsigned int i = 0; i < node->mNumChildren; i++)
  {
    // Recursion over the children
    process_node(node->mChildren[i], scene);
  }
}

void Model::draw(const Shader &shader) const
{
  // Set model transformation in shader
  shader.setMat4("model_matrix", pose.active_transformation());
  for (size_t i = 0; i < meshes.size(); i++)
  {
    meshes[i].draw(shader);
  }
}

} // namespace scigl_render
