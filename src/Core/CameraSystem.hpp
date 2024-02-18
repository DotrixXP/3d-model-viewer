#pragma once
#include "../GUI/WindowSystem.hpp"
#include "ArcBallCamera.hpp"
#include "FpsCamera.hpp"
#include <glm/glm.hpp>

enum Cameras
{
    STATIC,
    FIRST_PERSON,
};

class CameraSystem
{
  public:
    static CameraSystem &GetInstance();
    void SetActiveCamera(Cameras camera); // Nastaví aktivní kameru
    Cameras GetActiveCameraType();        // Vrátí aktivní kameru
    void ProcessMouseScrollInput(const float definedXOffset,
                                 const float definedYOffset);
    void ProcessMouseInput(const float definedXPos, const float definedYPos);
    const glm::mat4 GetViewMatrix() const;      // Vrátí view matici
    const glm::mat4 GetProjectionMatrix();      // Vrátí projekční matici
    const glm::mat4 GetOrthoProjectionMatrix(); // Vrátí projekční matici
    FpsCamera &GetFpsCamera();                  // Vrátí FPS kameru
    void UpdateInput(); // Aktualizuje vstupní data klávesnice
    void SetFov(const float fov, const glm::vec2 viewportSize);
    void SetProjMatToOrtho();
    void SetProjMatToPerspective(const glm::vec2 viewportSize);
    CameraSystem(const CameraSystem &) = delete;
    CameraSystem &operator=(const CameraSystem &) = delete;
    CameraSystem(CameraSystem &&) = delete;
    CameraSystem &operator=(CameraSystem &&) = delete;
    void SetInputState(bool state);
    void SetZoomMultiplier(float zoomMultiplier);
  private:
    CameraSystem() noexcept;
    ~CameraSystem() = default;

    Cameras m_activeCamera;
    float m_zoomMultiplier;
    FpsCamera m_fpsCamera;
    ArcBallCamera m_orbitalCamera;
    bool m_inputState;
};
