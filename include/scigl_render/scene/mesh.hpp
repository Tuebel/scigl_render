#pragma once
#include <scigl_render/shader/shader.hpp>
#include <scigl_render/scene/diffuse_texture.hpp>
#include <scigl_render/scene/vertex.hpp>
#include <scigl_render/scene/material.hpp>
#include <vector>
#include <assimp/scene.h>

namespace scigl_render
{
class Mesh
{
public:
  /*!
  Load the mesh from an assimp mesh.
  \param mesh the assimp mesh object point to the data in the scene
  \param scene contains the actual scene data
  */
public:
  Mesh(const aiMesh *mesh, const aiScene *scene);

  /*!
  Construct a mesh which is defined by its vertices, vertex indices and the
  textures. It is ready to be drawn.
  \param vertices for the VBO
  \param indices for the EBO
  \param textures for the mesh 
  */
  Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices,
       Material mat, DiffuseTexture text);

  /*!
 Renders this mesh.
 */
  void draw(const Shader &shader) const;

private:
  std::vector<Vertex> vertices;
  // indices for the EBO
  std::vector<GLuint> vertex_indices;
  // One material per mesh
  Material material;
  // Assume simple models with only one texture per mesh
  DiffuseTexture texture;
  GLuint VAO, VBO, EBO;

  /*!
  Call this method to initialize the loaded data in OpenGL.
  */
  void init();

  /*!
  Loads the textures for the specified material and type.
  \param material the assimp material for the mesh
  \param type the type of material (e.g. diffuse, specular, ...)
  \param type_name the string name of the type (e.g. texture_diffuse for
  aiTextureType_DIFFUSE)
  \return the textures that have been loaded from a file into OpenGL
  */
  DiffuseTexture load_diffuse_texture(const aiMaterial *material);

  /*!
  Loads the texture from the image file and stores it in the OpenGL context.
  \param path absolute path to the image file
  \return OpenGL id of the texture (from glGenTextures)
  */
  GLuint texture_from_file(const std::string &path);

  /*!
  Converts the assimp color to a glm vector that can be set as uniform in the
  shader.
  */
  glm::vec3 convert_color(const aiColor3D &color) const;
};
} // namespace scigl_render
