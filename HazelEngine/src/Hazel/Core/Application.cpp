#include "hzpch.h"
#include "Hazel/Core/Application.h"

#include <Hazel/Renderer/Renderer.h>
#include "Hazel/Core/Base.h"

#include <GLFW/glfw3.h>

namespace Hazel 
{
	Application* Application::s_Instance = nullptr;

	Application::Application(const std::string& name)
	{
		HZ_PROFILE_FUNCTION()
		
		HZ_CORE_ASSERT(!s_Instance, "Application already exists!")
		s_Instance = this;

		m_Window = Window::Create(WindowProps(name));
		m_Window->SetEventCallback(HZ_BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();
		
		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
		HZ_PROFILE_FUNCTION()
		
		Renderer::Shutdown();
		s_Instance = nullptr;
	}

	void Application::PushLayer(Layer* layer)
	{
		HZ_PROFILE_FUNCTION()
		
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		HZ_PROFILE_FUNCTION()
		
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::Close()
	{
		m_Running = false;
	}

	void Application::OnEvent(Event& e)
	{
		HZ_PROFILE_FUNCTION()
		
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(HZ_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(HZ_BIND_EVENT_FN(Application::OnWindowResize));

		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			if (e.Handled)
				break;
			(*it)->OnEvent(e);
		}
	}

	void Application::Run() 
	{
		HZ_PROFILE_FUNCTION()
		
		while (m_Running)
		{
			HZ_PROFILE_SCOPE("RunLoop")
			
			// Calculating Delta Time
			const auto time = static_cast<float>(glfwGetTime()); // TODO: Platform::GetTime();
			const Timestep timeStep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			// we only stop updating the layers when our app is minimized
			// that way Editor ImGUI controls are still available, and we don't take any CPU time to update our layers
			if (!m_Minimized)
			{
				{
					HZ_PROFILE_SCOPE("LayerStack OnUpdate")
					
					// Updating layer stack
					for (Layer* layer : m_LayerStack)
						layer->OnUpdate(timeStep);
				}

				{
					HZ_PROFILE_SCOPE("LayerStack OnImGuiRender")
					
					// Updating ImGUI Layer
					m_ImGuiLayer->Begin();
					for (Layer* layer : m_LayerStack)
						layer->OnImGuiRender();
					m_ImGuiLayer->End();
				}
			}

			// Updating the window
			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(const WindowCloseEvent& e)
	{
		Close();
		return true;
	}

	bool Application::OnWindowResize(const WindowResizeEvent& e)
	{
		HZ_PROFILE_FUNCTION()
		
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}
		
		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());
		
		return false;
	}
}
