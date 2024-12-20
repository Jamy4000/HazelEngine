#include "hzpch.h"
#include "OrthographicCameraController.h"

#include "Hazel/Core/KeyCodes.h"
#include "Hazel/Core/Input.h"

namespace Hazel
{
    OrthographicCameraController::OrthographicCameraController(const float aspectRatio, const bool rotation) :
        m_AspectRatio(aspectRatio),
        m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel),
        m_Rotation(rotation)
    {
    }

    void OrthographicCameraController::OnUpdate(const Timestep ts)
    {
		HZ_PROFILE_FUNCTION()
		
        // Camera Rotation
        if (m_Rotation)
        {
            if (Input::IsKeyPressed(HZ_KEY_Q))
            {
                m_CameraRotation += m_CameraRotationSpeed * ts;
            }
            if (Input::IsKeyPressed(HZ_KEY_E))
            {
                m_CameraRotation -= m_CameraRotationSpeed * ts;
            }
            
            if (m_CameraRotation > 180.0f)
                m_CameraRotation -= 360.0f;
            else if (m_CameraRotation <= -180.0f)
                m_CameraRotation += 360.0f;
            
            m_Camera.SetRotation(m_CameraRotation);
        }
        
        // Camera Movement
        if (Input::IsKeyPressed(HZ_KEY_A))
        {
            m_CameraPosition.x -= cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
            m_CameraPosition.y -= sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
        }
        else if (Input::IsKeyPressed(HZ_KEY_D))
        {
            m_CameraPosition.x += cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
            m_CameraPosition.y += sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
        }

        if (Input::IsKeyPressed(HZ_KEY_W))
        {
            m_CameraPosition.x += -sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
            m_CameraPosition.y += cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
        }
        else if (Input::IsKeyPressed(HZ_KEY_S))
        {
            m_CameraPosition.x -= -sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
            m_CameraPosition.y -= cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
        }

        m_CameraTranslationSpeed = m_ZoomLevel;
        m_Camera.SetPosition(m_CameraPosition);
    }

    void OrthographicCameraController::OnEvent(Event& e)
    {
		HZ_PROFILE_FUNCTION()
		
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<MouseScrolledEvent>(HZ_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
        dispatcher.Dispatch<WindowResizeEvent>(HZ_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
    }

    void OrthographicCameraController::OnResize(float width, float height)
    {
        m_AspectRatio = static_cast<float>(width) / static_cast<float>(height);
        CalculateView();
    }

    void OrthographicCameraController::SetZoomLevel(const float zoomLevel)
    {
        m_ZoomLevel = zoomLevel;
        CalculateView();
    }

    void OrthographicCameraController::CalculateView()
    {
        m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
    }

    bool OrthographicCameraController::OnMouseScrolled(const MouseScrolledEvent& e)
    {
		HZ_PROFILE_FUNCTION()
		
        m_ZoomLevel -= e.GetYOffset() * m_CameraZoomSpeed;
        m_ZoomLevel = std::max(m_ZoomLevel, m_MinCameraZoomLevel);
        CalculateView();
        return false;
    }

    bool OrthographicCameraController::OnWindowResized(const WindowResizeEvent& e)
    {
		HZ_PROFILE_FUNCTION()
		
        OnResize(static_cast<float>(e.GetWidth()), static_cast<float>(e.GetHeight()));
        return false;
    }
}
