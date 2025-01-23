#pragma once
#include "Model.hpp"
#include "../GUI/WindowSystem.hpp"
#include "../Graphics/Renderer.hpp"
#include <filesystem>
#include <future>

enum class PostProcessing : std::uint32_t {
    CALC_TANGENT_SPACE = aiProcess_CalcTangentSpace,
    JOIN_IDENTICAL_VERTICES = aiProcess_JoinIdenticalVertices,
    MAKE_LEFT_HANDED = aiProcess_MakeLeftHanded,
    TRIANGULATE = aiProcess_Triangulate,
    REMOVE_COMPONENT = aiProcess_RemoveComponent,
    GEN_NORMALS = aiProcess_GenNormals,
    GEN_SMOOTH_NORMALS = aiProcess_GenSmoothNormals,
    SPLIT_LARGE_MESHES = aiProcess_SplitLargeMeshes,
    PRE_TRANSFORM_VERTICES = aiProcess_PreTransformVertices,
    LIMIT_BONE_WEIGHTS = aiProcess_LimitBoneWeights,
    VALIDATE_DATA_STRUCTURE = aiProcess_ValidateDataStructure,
    IMPROVE_CACHE_LOCALITY = aiProcess_ImproveCacheLocality,
    REMOVE_REDUNDANT_MATERIALS = aiProcess_RemoveRedundantMaterials,
    FIX_INFACING_NORMALS = aiProcess_FixInfacingNormals,
    SORT_BY_PTYPE = aiProcess_SortByPType,
    FIND_DEGENERATES = aiProcess_FindDegenerates,
    FIND_INVALID_DATA = aiProcess_FindInvalidData,
    GEN_UV_COORDS = aiProcess_GenUVCoords,
    TRANSFORM_UV_COORDS = aiProcess_TransformUVCoords,
    FIND_INSTANCES = aiProcess_FindInstances,
    OPTIMIZE_MESHES = aiProcess_OptimizeMeshes,
    OPTIMIZE_GRAPH = aiProcess_OptimizeGraph,
    FLIP_UVS = aiProcess_FlipUVs,
    FLIP_WINDING_ORDER = aiProcess_FlipWindingOrder,
    SPLIT_BY_BONE_COUNT = aiProcess_SplitByBoneCount,
    DEBONE = aiProcess_Debone,
};

class ModelLoader {
  public:
    ModelLoader();
    ~ModelLoader() = default;
    void TryStartModelLoading(const std::filesystem::path& modelPath);
    std::optional<Model> RetrieveLoadedModel();

    void OptimizeModel(Model& model, PostProcessing effect);

  private:
    void ProcessNode(std::vector<Mesh>& meshes, aiNode* node, const aiScene* scene);
    Mesh ProcessMesh(aiMesh* mesh);
    Assimp::Importer m_importer;
    std::optional<std::future<std::tuple<const aiScene*, std::filesystem::path>>> m_loading;
};
