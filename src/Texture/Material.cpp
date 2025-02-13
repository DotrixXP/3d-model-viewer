#include "Material.hpp"

#include <utility>

MaterialSelection::MaterialSelection(std::variant<std::string, glm::vec3> diffuse,
                                     std::optional<std::string> specular,
                                     std::optional<std::string> normal)
    : diffuse(diffuse)
    , specular(std::move(specular))
    , normal(std::move(normal)) {}

MaterialSelection::MaterialSelection()
    : diffuse(std::nullopt)
    , specular(std::nullopt) {}

Material::Material(std::optional<std::variant<Texture, glm::vec3>> diffuse,
                   std::optional<Texture> specular)
    : diffuse(diffuse)
    , specular(specular) {}

Material::Material()
    : diffuse(std::nullopt)
    , specular(std::nullopt) {}

std::optional<glm::vec3> Material::TryGetDiffuseColor() const {
    if (diffuse.has_value() && std::holds_alternative<glm::vec3>(diffuse.value())) {
        return std::get<glm::vec3>(diffuse.value());
    }
    return std::nullopt;
}

std::optional<Texture> Material::TryGetNormalTexture() const {
    if (normal.has_value()) {
        return normal;
    }
    return std::nullopt;
}

std::optional<Texture> Material::TryGetDiffuseTexture() const {
    if (diffuse.has_value() && std::holds_alternative<Texture>(diffuse.value())) {
        return std::get<Texture>(diffuse.value());
    }
    return std::nullopt;
}

bool Material::IsValid() const {
    return diffuse.has_value();
}
