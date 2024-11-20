#include "Sandbox2D.h"

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"),
		m_CameraController(1280.0f / 720.0f),
		m_SquareColor({ 0.2f, 0.3f, 0.8f, 1.0f })
{
}

void Sandbox2D::OnAttach()
{
	HZ_PROFILE_FUNCTION()
	
	m_CheckerboardTexture = Hazel::Texture2D::Create("assets/textures/checkboard.jpg");
	
	// Init Particle
	m_Particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	m_Particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
	m_Particle.SizeBegin = 0.5f, m_Particle.SizeVariation = 0.3f, m_Particle.SizeEnd = 0.0f;
	m_Particle.MinLifeTime = 1.0f;
	m_Particle.Velocity = { 0.0f, 0.0f };
	m_Particle.VelocityVariation = { 3.0f, 1.0f };
	m_Particle.Position = { 0.0f, 0.0f };
}

void Sandbox2D::OnDetach()
{
	HZ_PROFILE_FUNCTION()
}

void Sandbox2D::OnUpdate(const Hazel::Timestep ts)
{
	HZ_PROFILE_FUNCTION()

	m_LastFrameDelta = ts.GetMilliseconds();
	
	// Update
	m_CameraController.OnUpdate(ts);

	// Render
	Hazel::Renderer2D::ResetStats();
	{
		HZ_PROFILE_SCOPE("Renderer Prep")
		Hazel::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
		Hazel::RenderCommand::Clear();
	}

	// Drawing
	{
		HZ_PROFILE_SCOPE("Drawing Scene")

		Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());

		// Simple Colored Quad
		Hazel::Renderer2D::DrawQuad({-1.0f, 0.0f}, {0.8f, 0.8f}, {0.2f, 0.8f, 0.3f, 1.0f});
		Hazel::Renderer2D::DrawQuad({0.5f, -0.5f}, {0.5f, 0.75f}, {0.8f, 0.2f, 0.3f, 1.0f});

		static float rotation = 0.0f;
		rotation += ts * 10.0f;
		Hazel::Renderer2D::DrawRotatedQuad({0.0f, 0.0f}, {0.5f, 0.75f}, 
			glm::radians(rotation), m_SquareColor);
		
		// Textured Quad
		Hazel::Renderer2D::DrawQuad({0.0f, 0.0f, -0.1f}, {20.0f, 20.0f},
			m_CheckerboardTexture, 10.0f, glm::vec4(0.2f, 0.8f, 0.5f, 1.0f));

		Hazel::Renderer2D::DrawQuad({0.0f, 0.0f, -0.1f}, {1.0f, 1.0f},
			m_CheckerboardTexture, 20.0f);
		
		Hazel::Renderer2D::DrawRotatedQuad({-2.0f, 0.0f, 0.0f}, {1.0f, 1.0f},
			glm::radians(-rotation), m_CheckerboardTexture, 20.0f);
		
		Hazel::Renderer2D::EndScene();

		
		Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());
		for (float y = -5.0f; y < 5.0f; y += 0.5f)
		{
			for (float x = -5.0f; x < 5.0f; x += 0.5f)
			{
				const glm::vec4 color = { (x + 5.0f) / 10.0f, 0.3f, (y + 5.0f) / 10.0f, 0.7f };
				Hazel::Renderer2D::DrawQuad({x, y}, {0.45f, 0.45f}, color);
			}
		}
		Hazel::Renderer2D::EndScene();
	}

	{
		// Created a Particle effect on Mouse click
		if (Hazel::Input::IsMouseButtonPressed(HZ_MOUSE_BUTTON_LEFT))
		{
			auto [x, y] = Hazel::Input::GetMousePosition();
			const auto width = Hazel::Application::Get().GetWindow().GetWidth();
			const auto height = Hazel::Application::Get().GetWindow().GetHeight();

			const auto bounds = m_CameraController.GetCamera().GetBounds();
			const auto pos = m_CameraController.GetCamera().GetPosition();
			x = (x / static_cast<float>(width)) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
			y = bounds.GetHeight() * 0.5f - (y / static_cast<float>(height)) * bounds.GetHeight();
			m_Particle.Position = { x + pos.x, y + pos.y };
			for (int i = 0; i < 5; i++)
				m_ParticleSystem.Emit(m_Particle);
		}

		m_ParticleSystem.OnUpdate(ts);
		m_ParticleSystem.OnRender(m_CameraController.GetCamera());
	}
}

void Sandbox2D::OnImGuiRender()
{
	HZ_PROFILE_FUNCTION()
	
	ImGui::Begin("Settings");

	const auto stats = Hazel::Renderer2D::GetStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quad: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Triangles: %d", stats.GetTotalTriangleCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

	ImGui::End();
}

void Sandbox2D::OnEvent(Hazel::Event& e)
{
	m_CameraController.OnEvent(e);
}