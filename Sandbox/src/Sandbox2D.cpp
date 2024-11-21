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
	
	m_CheckerboardTexture = Hazel::Texture2D::Create("assets/sandbox/textures/checkboard.jpg");

	m_CameraController.SetZoomLevel(7.5f);
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
}

void Sandbox2D::OnImGuiRender()
{
	HZ_PROFILE_FUNCTION()

	// Creating Dockspace
	{
		static bool s_DockspaceOpen = true;
	    static bool opt_fullscreen_persistant = true;
	    bool opt_fullscreen = opt_fullscreen_persistant;
	    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	    // because it would be confusing to have two docking targets within each others.
	    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	    if (opt_fullscreen)
	    {
	        ImGuiViewport* viewport = ImGui::GetMainViewport();
	        ImGui::SetNextWindowPos(viewport->GetWorkPos());
	        ImGui::SetNextWindowSize(viewport->GetWorkSize());
	        ImGui::SetNextWindowViewport(viewport->ID);
	        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	    }

	    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background 
	    // and handle the pass-thru hole, so we ask Begin() to not render a background.
	    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
	        window_flags |= ImGuiWindowFlags_NoBackground;

	    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
	    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
	    // all active windows docked into it will lose their parent and become undocked.
	    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
	    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
	    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	    ImGui::Begin("DockSpace Demo", &s_DockspaceOpen, window_flags);
	    ImGui::PopStyleVar();

	    if (opt_fullscreen)
	        ImGui::PopStyleVar(2);

	    // DockSpace
	    ImGuiIO& io = ImGui::GetIO();
	    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	    {
	        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
	        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	    }

	    if (ImGui::BeginMenuBar())
	    {
	        if (ImGui::BeginMenu("File"))
	        {
	            // Disabling fullscreen would allow the window to be moved to the front of other windows,
	            // which we can't undo at the moment without finer window depth/z control.
	            //ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

	            if (ImGui::MenuItem("Exit"))
	            {
		            Hazel::Application::Get().Close();
	            }
	            ImGui::EndMenu();
	        }

	        ImGui::EndMenuBar();
	    }

		
	
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

	    ImGui::End();
	}
}

void Sandbox2D::OnEvent(Hazel::Event& e)
{
	m_CameraController.OnEvent(e);
}