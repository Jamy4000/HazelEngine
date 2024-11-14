#include "hzpch.h"
#include "Hazel/Core/Application.h"

#include <Hazel/Renderer/Renderer.h>
#include "Hazel/Core/Core.h"

#include <GLFW/glfw3.h>

namespace Hazel 
{
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		HZ_CORE_ASSERT(!s_Instance, "Application already exists!")
		s_Instance = this;

		m_Window = Window::Create();
		m_Window->SetEventCallback(HZ_BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();
		
		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
		Renderer::Shutdown();
		s_Instance = nullptr;
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(HZ_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(HZ_BIND_EVENT_FN(Application::OnWindowResize));

		// goes backward so that the overlaying layers are prioritize (e.g. if UI is up, we don't want to shoot a gun)
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::Run() 
	{
		while (m_Running)
		{
			// Calculating Delta Time
			const auto time = static_cast<float>(glfwGetTime()); // TODO: Platform::GetTime();
			const Timestep timeStep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			// we only stop updating the layers when our app is minimized
			// that way Editor ImGUI controls are still available, and we don't take any CPU time to update our layers
			if (!m_Minimized)
			{
				// Updating layer stack
				for (Layer* layer : m_LayerStack)
					layer->OnUpdate(timeStep);
			}

			// Updating ImGUI Layer
			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			// Updating the window
			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(const WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(const WindowResizeEvent& e)
	{
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
