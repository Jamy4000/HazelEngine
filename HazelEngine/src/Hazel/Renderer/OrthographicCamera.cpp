#include "hzpch.h"
#include "Hazel/Renderer/OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Hazel
{
	OrthographicCamera::OrthographicCamera(const float left, const float right, const float bottom, const float top) :
		m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)),
		m_ViewMatrix(1.0f), 
		m_ViewProjectionMatrix(m_ProjectionMatrix * m_ViewMatrix),
		m_Bounds({ left, right, bottom, top }),
		m_Position(0.0f, 0.0f, 0.0f)
	{
		HZ_PROFILE_FUNCTION()
	}

	void OrthographicCamera::SetProjection(const float left, const float right, const float bottom, const float top)
	{
		HZ_PROFILE_FUNCTION()

		m_Bounds.Left = left;
		m_Bounds.Right = right;
		m_Bounds.Bottom = bottom;
		m_Bounds.Top = top;
		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::RecalculateViewMatrix()
	{
		HZ_PROFILE_FUNCTION()
		
		const glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));

		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}