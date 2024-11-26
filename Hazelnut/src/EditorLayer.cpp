#include "EditorLayer.h"

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Hazel
{
	EditorLayer::EditorLayer() : Layer("Editor"),
			m_CameraController(1280.0f / 720.0f),
			m_SquareColor({ 0.2f, 0.3f, 0.8f, 1.0f })
	{
	}

	void EditorLayer::OnAttach()
	{
		HZ_PROFILE_FUNCTION()
		
		m_CheckerboardTexture = Texture2D::Create("assets/sandbox/textures/checkboard.jpg");


		FrameBufferSpecification frameBufferSpecification;
		frameBufferSpecification.Width = 1280;
		frameBufferSpecification.Height = 720;
		m_FrameBuffer = FrameBuffer::Create(frameBufferSpecification);
		
		m_CameraController.SetZoomLevel(7.5f);
	}

	void EditorLayer::OnDetach()
	{
		HZ_PROFILE_FUNCTION()
	}

	void EditorLayer::OnUpdate(const Timestep ts)
	{
		HZ_PROFILE_FUNCTION()

		const FrameBufferSpecification spec = m_FrameBuffer->GetSpecifications();
		const auto width = static_cast<uint32_t>(m_ViewportSize.x);
		const auto height = static_cast<uint32_t>(m_ViewportSize.y);
		if (m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
			(spec.Width != width || spec.Height != height))
		{
			m_FrameBuffer->Resize(width, height);
			m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);
		}
		
		// Update
		if (m_ViewportFocused)
			m_CameraController.OnUpdate(ts);

		// Render
		Renderer2D::ResetStats();
		{
			HZ_PROFILE_SCOPE("Renderer Prep")
			m_FrameBuffer->Bind();
			RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
			RenderCommand::Clear();
		}

		// Drawing
		{
			HZ_PROFILE_SCOPE("Drawing Scene")

			Renderer2D::BeginScene(m_CameraController.GetCamera());

			// Simple Colored Quad
			Renderer2D::DrawQuad({-1.0f, 0.0f}, {0.8f, 0.8f}, {0.2f, 0.8f, 0.3f, 1.0f});
			Renderer2D::DrawQuad({0.5f, -0.5f}, {0.5f, 0.75f}, {0.8f, 0.2f, 0.3f, 1.0f});

			static float rotation = 0.0f;
			rotation += ts * 10.0f;
			Renderer2D::DrawRotatedQuad({0.0f, 0.0f}, {0.5f, 0.75f}, 
				glm::radians(rotation), m_SquareColor);
			
			// Textured Quad
			Renderer2D::DrawQuad({0.0f, 0.0f, -0.1f}, {20.0f, 20.0f},
				m_CheckerboardTexture, 10.0f, glm::vec4(0.2f, 0.8f, 0.5f, 1.0f));

			Renderer2D::DrawQuad({0.0f, 0.0f, -0.1f}, {1.0f, 1.0f},
				m_CheckerboardTexture, 20.0f);
			
			Renderer2D::DrawRotatedQuad({-2.0f, 0.0f, 0.0f}, {1.0f, 1.0f},
				glm::radians(-rotation), m_CheckerboardTexture, 20.0f);
			
			Renderer2D::EndScene();

			
			Renderer2D::BeginScene(m_CameraController.GetCamera());
			for (float y = -5.0f; y < 5.0f; y += 0.5f)
			{
				for (float x = -5.0f; x < 5.0f; x += 0.5f)
				{
					const glm::vec4 color = { (x + 5.0f) / 10.0f, 0.3f, (y + 5.0f) / 10.0f, 0.7f };
					Renderer2D::DrawQuad({x, y}, {0.45f, 0.45f}, color);
				}
			}
			Renderer2D::EndScene();
			m_FrameBuffer->Unbind();
		}
	}

	void EditorLayer::OnImGuiRender()
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
		    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DockingEnable)
		    {
			    const ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
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
			            Application::Get().Close();
		            }
		            ImGui::EndMenu();
		        }

		        ImGui::EndMenuBar();
		    }
		}

		// Setting Window
		{
			ImGui::Begin("Settings");

			const auto stats = Renderer2D::GetStats();
			ImGui::Text("Renderer2D Stats:");
			ImGui::Text("Draw Calls: %d", stats.DrawCalls);
			ImGui::Text("Quad: %d", stats.QuadCount);
			ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
			ImGui::Text("Triangles: %d", stats.GetTotalTriangleCount());
			ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

			ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
			
			ImGui::End();
		}

		// Starting Viewport
		{
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
			
			ImGui::Begin("Viewport");

			m_ViewportFocused = ImGui::IsWindowFocused();
			m_ViewportHovered = ImGui::IsWindowHovered();
			Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);

			// Checking Viewport Resizing
			const ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
			m_ViewportSize = glm::vec2(viewportPanelSize.x, viewportPanelSize.y);

			const uint32_t textureID = m_FrameBuffer->GetColorAttachmentRendererID();
			ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_ViewportSize.x, m_ViewportSize.y },
				ImVec2{ 0.0f, 1.0f}, ImVec2{ 1.0f, 0.0f });

			ImGui::End();
			ImGui::PopStyleVar();
		}

		// Ending Dockspace
	    ImGui::End();
	}

	void EditorLayer::OnEvent(Event& e)
	{
		m_CameraController.OnEvent(e);
	}
}