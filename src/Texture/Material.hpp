#pragma once
#include <glm/glm.hpp>
#include <optional>
#include <string>
#include <variant>
#include "Texture.hpp"

struct MaterialSelection {
    std::optional<std::variant<std::string, glm::vec3>> diffuse;
    std::optional<std::string> specular;
    std::optional<std::string> normal;

    MaterialSelection(std::variant<std::string, glm::vec3>,
                      std::optional<std::string> specular,
                      std::optional<std::string> normal);

    MaterialSelection();
};

struct Material {
    std::optional<std::variant<Texture, glm::vec3>> diffuse;
    std::optional<Texture> specular;
    std::optional<Texture> normal;

    [[nodiscard]] std::optional<Texture> TryGetNormalTexture() const;
    [[nodiscard]] std::optional<glm::vec3> TryGetDiffuseColor() const;
    [[nodiscard]] std::optional<Texture> TryGetDiffuseTexture() const;

    [[nodiscard]] bool IsValid() const;

    Material(std::optional<std::variant<Texture, glm::vec3>> diffuse,
             std::optional<Texture> specular);
    Material();
};
