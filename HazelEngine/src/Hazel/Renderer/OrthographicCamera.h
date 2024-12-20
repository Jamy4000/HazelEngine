#pragma once

#include <glm/glm.hpp>

namespace Hazel
{
	struct OrthographicCameraBounds
	{
		float Left, Right;
		float Bottom, Top;

		[[nodiscard]] float GetWidth() const { return Right - Left; }
		[[nodiscard]] float GetHeight() const { return Top - Bottom; }
	};
    
	class OrthographicCamera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);

		void SetProjection(float left, float right, float bottom, float top);

		[[nodiscard]] const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(const glm::vec3& position)
		{
			m_Position = position; 
			RecalculateViewMatrix();
		}

		[[nodiscard]] float GetRotation() const { return m_Rotation; }
		void SetRotation(const float rotation)
		{
			m_Rotation = rotation; 
			RecalculateViewMatrix();
		}
		
		[[nodiscard]] const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		[[nodiscard]] const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		[[nodiscard]] const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
		[[nodiscard]] const OrthographicCameraBounds& GetBounds() const { return m_Bounds; }

	private:
		void RecalculateViewMatrix();

	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;
		OrthographicCameraBounds m_Bounds;

		glm::vec3 m_Position;
		float m_Rotation = 0.0f;
	};
}