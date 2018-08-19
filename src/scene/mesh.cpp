#include <scigl_render/scene/mesh.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <scigl_render/scene/stb_image.h>
#include <iostream>

namespace scigl_render
{
Mesh::Mesh(const aiMesh *mesh, const aiScene *scene)
{
  // vertices
  for (unsigned int i = 0; i < mesh->mNumVertices; i++)
  {
    // Convert the assimp vertices
    Vertex vertex;
    // positions
    vertex.position.x = mesh->mVertices[i].x;
    vertex.position.y = mesh->mVertices[i].y;
    vertex.position.z = mesh->mVertices[i].z;
    // normals
    if (mesh->HasNormals())
    {
      vertex.normal.x = mesh->mNormals[i].x;
      vertex.normal.y = mesh->mNormals[i].y;
      vertex.normal.z = mesh->mNormals[i].z;
    }
    else
    {
      vertex.normal = glm::vec3(0, 0, 0);
    }
    // texture coordinates
    if (mesh->HasTextureCoords(0))
    {
      vertex.texture_coordinates.x = mesh->mTextureCoords[0][i].x;
      vertex.texture_coordinates.y = mesh->mTextureCoords[0][i].y;
    }
    else
    {
      vertex.texture_coordinates = glm::vec2(0, 0);
    }
    // add vertex
    vertices.push_back(vertex);
  }
  // indices of vertices
  for (unsigned int i = 0; i < mesh->mNumFaces; i++)
  {
    aiFace face = mesh->mFaces[i];
    for (unsigned int j = 0; j < face.mNumIndices; j++)
    {
      vertex_indices.push_back(face.mIndices[j]);
    }
  }
  // material
  material = {};
  DiffuseTexture texture;
  if (scene->HasMaterials())
  {
    // material properties
    const aiMaterial *ai_material = scene->mMaterials[mesh->mMaterialIndex];
    aiColor3D ambient_color;
    if (AI_SUCCESS == ai_material->Get(AI_MATKEY_COLOR_AMBIENT, ambient_color))
    {
      material.ambient = convert_color(ambient_color);
    }
    aiColor3D diffuse_color;
    if (AI_SUCCESS == ai_material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse_color))
    {
      material.diffuse = convert_color(diffuse_color);
    }
    aiColor3D specular_color;
    if (AI_SUCCESS == ai_material->Get(AI_MATKEY_COLOR_DIFFUSE, specular_color))
    {
      material.specular = convert_color(specular_color);
    }
    float shininess;
    if (AI_SUCCESS == ai_material->Get(AI_MATKEY_SHININESS, shininess))
    {
      material.shininess = shininess;
    }
    texture = load_diffuse_texture(ai_material);
  }
  else
  {
    // invalid texture
    texture.id = 0;
  }
  init();
}

Mesh::Mesh(std::vector<Vertex> vert, std::vector<GLuint> indi, Material mat,
           DiffuseTexture text)
    : vertices(std::move(vert)),
      vertex_indices(std::move(indi)),
      material(std::move(mat)),
      texture(std::move(text))
{
  init();
}

void Mesh::init()
{
  // vertex array
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);
  glGenBuffers(1, &VBO);
  // vertex buffer
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
               vertices.data(), GL_STATIC_DRAW);
  // element buffer (indices)
  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertex_indices.size() * sizeof(GLuint),
               vertex_indices.data(), GL_STATIC_DRAW);
  /*** attributes ***/
  // positions
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        reinterpret_cast<GLvoid *>(0));
  // normals
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        reinterpret_cast<GLvoid *>(offsetof(Vertex, normal)));
  // vertex texture coords
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        reinterpret_cast<GLvoid *>(
                            offsetof(Vertex, texture_coordinates)));
  // Avoid modification of VAO
  glBindVertexArray(0);
}

void Mesh::draw(const Shader &shader) const
{
  shader.activate();
  shader.setVec3("material.ambient", material.ambient);
  shader.setVec3("material.diffuse", material.diffuse);
  shader.setVec3("material.specular", material.specular);
  shader.setFloat("material.shininess", material.shininess);
  // Shader shall sample form texture0
  shader.setInt("texture0", 0);
  shader.setFloat("diffuse_texture_strength", texture.strength);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture.id);
  // draw the vertex array
  glBindVertexArray(VAO);
  // do not use indices since we are using an EBO
  glDrawElements(GL_TRIANGLES, vertex_indices.size(), GL_UNSIGNED_INT, 0);
  // Reset
  glBindVertexArray(0);
}

DiffuseTexture Mesh::load_diffuse_texture(const aiMaterial *material)
{
  DiffuseTexture texture;
  if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
  {
    aiString filename;
    float strength;
    material->GetTexture(aiTextureType_DIFFUSE, 0, &filename, NULL, NULL,
                         &strength);
    texture.id = texture_from_file(filename.C_Str());
    texture.strength = strength;
  }
  else
  {
    // invalid texture
    texture.id = 0;
    std::cout << "material has no texture\n";
  }
  return texture;
}

GLuint Mesh::texture_from_file(const std::string &path)
{
  int width, height, color_component_count;
  stbi_uc *data = stbi_load(path.c_str(), &width, &height,
                            &color_component_count, 0);
  std::cout << "Loaded texture from: " << path << "\n";
  if (!data)
  {
    stbi_image_free(data);
    throw std::runtime_error("Texture failed to load at path: " + path);
  }
  else
  {
    // Load the texture into OpenGL
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    // image format depends on number of color components.
    GLenum image_format = GL_RGB;
    if (color_component_count == 1)
    {
      image_format = GL_RED;
    }
    else if (color_component_count == 4)
    {
      image_format = GL_RGBA;
    }
    // Assign image to texture and generate Mipmap
    glTexStorage2D(GL_TEXTURE_2D, 1, image_format, width, height);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, image_format,
                    GL_UNSIGNED_BYTE, data);
    // Wrapping by repeating
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Linear interpolation between the pixels
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Clenup
    stbi_image_free(data);
    glBindTexture(GL_PROXY_TEXTURE_2D, 0);
    return textureID;
  }
}

glm::vec3 Mesh::convert_color(const aiColor3D &color) const
{
  return glm::vec3(color.r, color.g, color.b);
}
} // namespace scigl_render
