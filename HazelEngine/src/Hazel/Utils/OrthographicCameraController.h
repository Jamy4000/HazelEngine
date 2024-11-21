#pragma once

#include "Hazel/Renderer/OrthographicCamera.h"
#include "Hazel/Core/Timestep.h"

#include "Hazel/Events/ApplicationEvent.h" 
#include "Hazel/Events/MouseEvent.h" 

namespace Hazel
{
    // Wrapper around the OrthographicCamera class to control the camera
    class OrthographicCameraController
    {
    public:
        explicit OrthographicCameraController(float aspectRatio, bool rotation = false);

        void OnUpdate(Timestep ts);
        void OnEvent(Event& e);
        
        void OnResize(float width, float height);
        
        [[nodiscard]] OrthographicCamera& GetCamera() { return m_Camera; }
        [[nodiscard]] const OrthographicCamera& GetCamera() const { return m_Camera; }

        [[nodiscard]] float GetZoomLevel() const { return m_ZoomLevel; }
        void SetZoomLevel(float zoomLevel);

    private:
        void CalculateView();

        bool OnMouseScrolled(const MouseScrolledEvent& e);
        bool OnWindowResized(const WindowResizeEvent& e);
        
    private:
        float m_AspectRatio;
        float m_ZoomLevel = 1.0f;
        OrthographicCamera m_Camera;

        bool m_Rotation = false;
        float m_CameraRotation = 0.0f;  //In degrees, in the anti-clockwise direction
        float m_CameraRotationSpeed = 180.0f;

        glm::vec3 m_CameraPosition = {0.0f, 0.0f, 0.0f};
        float m_CameraTranslationSpeed = 3.0f;
        
        float m_CameraZoomSpeed = 0.25f;
        float m_MinCameraZoomLevel = 0.25f;
    };
}
