#include "Model.hpp"

Model::Model(const std::string &directoryPath, bool manualySetTextures) // .obj
{
  m_directoryPath = directoryPath;
  m_manualySetTextures = manualySetTextures;
  Model::LoadModel(directoryPath);

  m_position = glm::vec3(0.f);
  m_rotationDirection = glm::vec3(1.f);
  m_rotationAngle = 0;
}

void Model::DrawArrays(Shader &shader) {
  m_model = glm::mat4(1.0f);
  m_model = glm::translate(m_model, m_position);
  m_model =
      glm::rotate(m_model, glm::radians(m_rotationAngle), m_rotationDirection);
  // m_model = glm::scale(m_model, m_scale);
  shader.SetUniform("model", m_model);
  for (unsigned int i = 0; i < m_meshes.size();
       i++) // Každý mesh ve vectoru vykreslit
    m_meshes[i].DrawArrays(shader, m_manualySetTextures);
}

void Model::DrawArraysWithModelMatrix(Shader &shader) {
  for (unsigned int i = 0; i < m_meshes.size();
       i++) // Každý mesh ve vectoru vykreslit
    m_meshes[i].DrawArrays(shader, m_manualySetTextures);
}

void Model::LoadModel(std::string directoryPath) {
  Assimp::Importer importer;
  const aiScene *scene =
      importer.ReadFile(directoryPath, aiProcess_Triangulate);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
      !scene->mRootNode) // Pokud scene == null
  {
    Log::LogError(importer.GetErrorString());
    return;
  } else
    Log::LogInfo("Model: " + directoryPath + " uspesne nacten!");

  m_directoryPath =
      m_directoryPath.substr(0, m_directoryPath.find_last_of('/'));

  Model::ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode *node, const aiScene *scene) {
  // process all the node's meshes (if any)
  for (unsigned int i = 0; i < node->mNumMeshes; i++) // Dokud i != počtu Meshů
  {
    aiMesh *mesh = scene->mMeshes[node->mMeshes[i]]; // získáni meshe ze scény
    m_meshes.push_back(
        ProcessMesh(mesh, scene)); // přidání procesovaného meshe do vectoru
  }
  // then do the same for each of its children
  for (unsigned int i = 0; i < node->mNumChildren; i++) {
    ProcessNode(node->mChildren[i], scene); // Zanoří se do této funkce
  }
}

Mesh Model::ProcessMesh(aiMesh *mesh,
                        const aiScene *scene) // Z meshů získat vertexy
{
  std::vector<MeshData::Vertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<MeshData::Texture> textures;

  aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

  // walk through each of the mesh's vertices
  for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
    MeshData::Vertex vertex;
    glm::vec3 verticesVector;

    // získání positions
    verticesVector.x = mesh->mVertices[i].x;
    verticesVector.y = mesh->mVertices[i].y;
    verticesVector.z = mesh->mVertices[i].z;
    vertex.position = verticesVector;

    // získání normals
    if (mesh->HasNormals()) {
      verticesVector.x = mesh->mNormals[i].x;
      verticesVector.y = mesh->mNormals[i].y;
      verticesVector.z = mesh->mNormals[i].z;
      vertex.normal = verticesVector;
    }
    // získání texture coordinates
    if (mesh->mTextureCoords[0]) // Je v modelu texture souřadnice?
    {                            // Objekt může mít vicero texture souřadnic
      glm::vec2 texturesVector;  // 2d souřadnice
      texturesVector.x = mesh->mTextureCoords[0][i].x;
      texturesVector.y = mesh->mTextureCoords[0][i].y;
      vertex.texCoords = texturesVector;
    } else
      vertex.texCoords =
          glm::vec2(0.0f, 0.0f); // Pokud nejsou souřadnice, tak 0,0

    vertices.push_back(vertex); // Uložit získané vertexy do vectoru
  }

  // získání indices
  for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
    aiFace face = mesh->mFaces[i];

    // Uložit získané indices do indices vectoru
    for (unsigned int j = 0; j < face.mNumIndices; j++)
      indices.push_back(face.mIndices[j]);
  }

  if (!m_manualySetTextures) {
    // Získat textury a mapy
    if (mesh->mMaterialIndex >= 0) {
      aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
      std::vector<MeshData::Texture> diffuseMaps =
          LoadMaterialTextures(material, aiTextureType_DIFFUSE, "diffuse");
      textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
      std::vector<MeshData::Texture> specularMaps =
          LoadMaterialTextures(material, aiTextureType_SPECULAR, "specular");
      textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }
  }

  return Mesh(vertices, indices,
              textures); // Vrátí celý jeden Mesh (část modelu)
}

void Model::OverwriteTexture(uint32_t texture) {
  m_loadedTextures.clear();
  m_loadedTexturesPaths.clear();
  m_loadedTextures.push_back({texture, "diffuse"});
}

std::vector<MeshData::Texture>
Model::LoadMaterialTextures(aiMaterial *mat, aiTextureType type,
                            std::string typeName) {
  std::vector<MeshData::Texture> texturesToReturn;
  bool skip = false;
  aiString str;
  std::string filename;

  for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
    mat->GetTexture(type, i, &str);
    filename = str.C_Str();
    filename =
        filename.substr(filename.find_last_of('\\') + 1, filename.length());
    filename = m_directoryPath + "/textures/" + filename;

    for (unsigned int j = 0; j < m_loadedTextures.size(); j++) {
      if (m_loadedTexturesPaths[j] == str.C_Str()) {
        texturesToReturn.push_back(
            m_loadedTextures[j]); // Vrátí texturu co byla první použita
        skip = true;
        break;
      }
    }
  }
  if (!skip && std::string(str.C_Str()).find_last_of('\\') < 1000) {
    MeshData::Texture texture;
    m_loadedTexturesPaths.push_back(str.C_Str());

    texture.id = loadTexture(filename, GL_LINEAR);
    texture.type = typeName;

    texturesToReturn.push_back(texture);
    m_loadedTextures.push_back(texture);
  }

  return texturesToReturn;
}

void Model::SetScale(glm::vec3 scale) {
  // std::cout << "Upozorneni! Model: U slozitejsich modelu se nemusi zmena
  // scale faktoru projevit!\n";
  m_scale = glm::vec3(scale.x, scale.y, scale.z);
}

void Model::SetScale(float x, float y, float z) {
  // std::cout << "Upozorneni! Model: U slozitejsich modelu se nemusi zmena
  // scale faktoru projevit!\n";
  m_scale = glm::vec3(x, y, z);
}

Model::~Model() {
  for (int i = 0; i < m_meshes.size(); i++) {
    m_meshes[i].Destroy();
  }
}
