#pragma once
#include <glm/glm.hpp>
#include <optional>
#include <string>
#include <variant>
#include "Texture.hpp"

struct MaterialSelection {
    std::optional<std::variant<std::string, glm::vec3>> diffuse;
    std::optional<std::string> specular;

    MaterialSelection(std::variant<std::string, glm::vec3>, std::optional<std::string> specular);
    MaterialSelection();
};

struct Material {
    std::optional<std::variant<Texture, glm::vec3>> diffuse;
    std::optional<Texture> specular;
    
    [[nodiscard]] std::optional<glm::vec3> TryGetDiffuseColor() const;
    [[nodiscard]] std::optional<Texture> TryGetDiffuseTexture() const;
    void SetDiffuse(std::variant<Texture, glm::vec3> diffuse);
    void SetSpecular(Texture specular);
    [[nodiscard]] bool IsValid() const;

    Material(std::optional<std::variant<Texture, glm::vec3>> diffuse,
             std::optional<Texture> specular);
    Material();
};
