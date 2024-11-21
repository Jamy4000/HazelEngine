#pragma once

#include "Hazel/Core/Window.h"
#include "Hazel/Core/LayerStack.h"
#include "Hazel/Events/Event.h"
#include "Hazel/Events/ApplicationEvent.h"

#include "Hazel/ImGui/ImGuiLayer.h"

int main(int argc, char** argv);

namespace Hazel
{
	class Application
	{
	public:
		Application(const std::string& name = "Hazel App");
		virtual ~Application();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		void Close();
		
		[[nodiscard]] static Application& Get() { return *s_Instance; }

		[[nodiscard]] Window& GetWindow() const { return *m_Window; }
		
	private:
		void Run();
		bool OnWindowClose(const WindowCloseEvent& e);
		bool OnWindowResize(const WindowResizeEvent& e);

	private:
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;

		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;

		float m_LastFrameTime = 0.0f;

	private: // static
		static Application* s_Instance;
		friend int ::main(int argc, char** argv);
	};

	// To be defined in the client
	Application* CreateApplication();
}