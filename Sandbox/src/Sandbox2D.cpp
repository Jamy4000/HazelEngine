#include "Sandbox2D.h"

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
{
}

void Sandbox2D::OnAttach()
{
	HZ_PROFILE_FUNCTION()
	
	m_CheckerboardTexture = Hazel::Texture2D::Create("assets/textures/checkboard.jpg");
}

void Sandbox2D::OnDetach()
{
	HZ_PROFILE_FUNCTION()
}

void Sandbox2D::OnUpdate(const Hazel::Timestep ts)
{
	HZ_PROFILE_FUNCTION()
	
	// Update
	m_CameraController.OnUpdate(ts);

	// Render
	{
		HZ_PROFILE_SCOPE("Renderer Prep")
		Hazel::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
		Hazel::RenderCommand::Clear();
	}

	// Drawing
	{
		HZ_PROFILE_SCOPE("Drawing Scene")
		Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());
	
		Hazel::Renderer2D::DrawQuad({-1.0f, 0.0f}, {0.8f, 0.8f}, {0.2f, 0.8f, 0.3f, 1.0f});
		Hazel::Renderer2D::DrawQuad({0.5f, -0.5f}, {0.5f, 0.75f}, {0.8f, 0.2f, 0.3f, 1.0f});
	
		Hazel::Renderer2D::DrawQuad({0.0f, 0.0f, -0.1f}, {10.0f, 10.0f}, m_CheckerboardTexture);
	
		Hazel::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender()
{
	HZ_PROFILE_FUNCTION()
	
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	
	ImGui::End();
}

void Sandbox2D::OnEvent(Hazel::Event& e)
{
	m_CameraController.OnEvent(e);
}