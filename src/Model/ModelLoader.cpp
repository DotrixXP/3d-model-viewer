#include "ModelLoader.hpp"
#include <assimp/postprocess.h>

ModelLoader::ModelLoader()
    : m_importer() {}

std::optional<Model> ModelLoader::LoadModel(const std::filesystem::path& modelPath) {
    std::vector<Mesh> meshes;
    const aiScene* scene = m_importer.ReadFile(modelPath, aiProcess_Triangulate);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        Log::Error(m_importer.GetErrorString());
        return std::nullopt;
    }
    else {
        Log::Info("Model: ", modelPath, " successfully loaded!");
    }

    ModelLoader::ProcessNode(meshes, scene->mRootNode, scene);

    Model model(std::move(meshes), scene, modelPath);

    return { std::move(model) };
}

void ModelLoader::OptimizeModel(Model& model, PostProcessing effect) {
    std::vector<Mesh> meshes;
    const aiScene* scene = m_importer.ReadFile(model.m_path, (aiPostProcessSteps)effect);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        Log::Error(m_importer.GetErrorString());
    }
    else {
        Log::Info("Model: ", model.m_path, " successfully reloaded!");
    }

    ModelLoader::ProcessNode(meshes, scene->mRootNode, scene);

    for (auto mesh : model.m_meshes)
        mesh.Destroy();

    model.m_meshes = meshes;
    model.m_scene = scene;
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
