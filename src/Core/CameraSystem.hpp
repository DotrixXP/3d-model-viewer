#pragma once
#include "../GUI/WindowSystem.hpp"
#include "ArcBallCamera.hpp"
#include "FpsCamera.hpp"
#include <glm/glm.hpp>

enum class Cameras : uint8_t {
    ARCBALL,
    FIRST_PERSON,
};

class CameraSystem {
  public:
    static CameraSystem& GetInstance();
    void SetActiveCamera(Cameras camera); // Nastaví aktivní kameru
    Cameras GetActiveCameraType();        // Vrátí aktivní kameru
    void ProcessMouseScrollInput(const float definedYOffset);
    void ProcessMouseInput(const float definedXPos, const float definedYPos);
    [[nodiscard]] const glm::mat4 GetViewMatrix() const; // Vrátí view matici
    const glm::mat4 GetProjectionMatrix();               // Vrátí projekční matici
    const glm::mat4 GetOrthoProjectionMatrix();          // Vrátí projekční matici
    void UpdateInput();                                  // Aktualizuje vstupní data klávesnice
    void SetFov(const float fov);
    void SetProjMatToOrtho();
    void SetProjMatToPerspective();
    CameraSystem(const CameraSystem&) = delete;
    CameraSystem& operator=(const CameraSystem&) = delete;
    CameraSystem(CameraSystem&&) = delete;
    CameraSystem& operator=(CameraSystem&&) = delete;
    void SetInputState(bool state);
    void SetZoomMultiplier(float zoomMultiplier);
    float GetFov();

  private:
    CameraSystem() noexcept;
    ~CameraSystem() = default;

    Cameras m_activeCamera;
    float m_zoomMultiplier;
    FpsCamera m_fpsCamera;
    ArcBallCamera m_orbitalCamera;
    bool m_inputState;
};
