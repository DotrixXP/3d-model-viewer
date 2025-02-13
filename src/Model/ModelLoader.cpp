#include "ModelLoader.hpp"
#include <assimp/postprocess.h>
#include <cmath>

ModelLoader::ModelLoader()
    : m_importer()
    , m_loading() {}

std::uint32_t ModelLoader::s_postProcessingOptions = 0;

void ModelLoader::TryStartModelLoading(const std::filesystem::path& modelPath) {
    // Returning only scene because creating opengl buffers cannot be done in parallel
    if (!m_loading.has_value()) {
        m_loading = std::async(
            std::launch::async, [=, this]() -> std::tuple<const aiScene*, std::filesystem::path> {
                Log::Info("Loading model in async: ", modelPath);
                const aiScene* scene = m_importer.ReadFile(
                    modelPath.string().c_str(),
                    aiProcess_Triangulate | aiProcess_CalcTangentSpace | s_postProcessingOptions);

                if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
                    Log::Error(m_importer.GetErrorString());
                    return std::make_tuple(nullptr, modelPath);
                }
                else {
                    Log::Info("Model: ", modelPath, " successfully loaded!");
                }

                return std::make_tuple(scene, modelPath);
            });
    }
}

void ModelLoader::SetPostProcessingOptions(std::uint32_t options) {
    s_postProcessingOptions = options;
}

std::optional<Model> ModelLoader::RetrieveLoadedModel() {
    if (m_loading.has_value()) {
        if (m_loading.value().valid()) {
            try {
                auto [scene, path] = m_loading.value().get();
                m_loading.reset();
                if (scene) {
                    std::vector<Mesh> meshes;
                    ModelLoader::ProcessNode(meshes, scene->mRootNode, scene);

                    Model model(std::move(meshes), scene, path);

                    return std::optional<Model> { std::move(model) };
                }
                else {
                    Log::Error("Model loading failed");
                }
            }
            catch (const std::future_error& e) {
                Log::Error("Future error: ", e.what());
            }
        }
    }
    return std::nullopt;
}

void ModelLoader::ProcessNode(std::vector<Mesh>& meshes, aiNode* node, const aiScene* scene) {
    for (uint32_t i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(ProcessMesh(mesh));
    }

    for (uint32_t i = 0; i < node->mNumChildren; i++) {
        ProcessNode(meshes, node->mChildren[i], scene);
    }
}

Mesh ModelLoader::ProcessMesh(aiMesh* mesh) {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    for (uint32_t i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        glm::vec3 verticesVector;

        verticesVector.x = mesh->mVertices[i].x;
        verticesVector.y = mesh->mVertices[i].y;
        verticesVector.z = mesh->mVertices[i].z;
        vertex.position = verticesVector;

        if (mesh->HasNormals()) {
            verticesVector.x = mesh->mNormals[i].x;
            verticesVector.y = mesh->mNormals[i].y;
            verticesVector.z = mesh->mNormals[i].z;
            vertex.normal = verticesVector;
        }
        else {
            vertex.normal = glm::vec3(0.0f);
        }
        if (mesh->HasTangentsAndBitangents()) {
            verticesVector.x = mesh->mTangents[i].x;
            verticesVector.y = mesh->mTangents[i].y;
            verticesVector.z = mesh->mTangents[i].z;
            vertex.tangent = verticesVector;

            verticesVector.x = mesh->mBitangents[i].x;
            verticesVector.y = mesh->mBitangents[i].y;
            verticesVector.z = mesh->mBitangents[i].z;
            vertex.bitangent = verticesVector;
        }
        else {
            vertex.tangent = glm::vec3(0.0f);
            vertex.bitangent = glm::vec3(0.0f);
        }
        if (mesh->mTextureCoords[0]) {
            glm::vec2 texturesVector;
            texturesVector.x = mesh->mTextureCoords[0][i].x;
            texturesVector.y = mesh->mTextureCoords[0][i].y;
            vertex.texCoords = texturesVector;
        }
        else {
            vertex.texCoords = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }

    for (uint32_t i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (uint32_t j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    return { vertices, indices };
}
