#include "hzpch.h"
#include "WindowsWindow.h"

#include "Hazel/Events/MouseEvent.h"
#include "Hazel/Events/KeyEvent.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace Hazel
{
	static uint8_t s_GLFWWindowCount = 0;

	static void GLFWErrorCallback(int error, const char* description)
	{
		HZ_CORE_FATAL("GLFW Error ({0}): {1}", error, description);
	}

	Scope<Window> Window::Create(const WindowProps& props)
	{
		return CreateScope<WindowsWindow>(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props) : m_Window(nullptr), m_Context(nullptr)
	{
		HZ_PROFILE_FUNCTION()
		
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		HZ_PROFILE_FUNCTION()
		
		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		HZ_PROFILE_FUNCTION()
		
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		HZ_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		if (s_GLFWWindowCount == 0)
		{
			HZ_PROFILE_SCOPE("glfwInit")
			const int success = glfwInit();
			HZ_CORE_ASSERT(success, "Could not initialize GLFW!")
			glfwSetErrorCallback(GLFWErrorCallback);
		}

		{
			HZ_PROFILE_SCOPE("glfwCreateWindow")
			m_Window = glfwCreateWindow(static_cast<int>(props.Width), static_cast<int>(props.Height),
				props.Title.c_str(), nullptr, nullptr);
			++s_GLFWWindowCount;
		}

		m_Context = GraphicsContext::Create(m_Window);
		m_Context->Init();

		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

		// Set GLFW Callbacks
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, const int width, const int height)
		{
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			data.Width = width;
			data.Height = height;

			WindowResizeEvent event(width, height);
			data.EventCallback(event);
		});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
		{
			const WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			WindowCloseEvent event;
			data.EventCallback(event);
		});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, const int key,
			const int scanCode, const int action, const int mods)
		{
			const WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			switch (action)
			{
				case GLFW_PRESS:
				{
					KeyPressedEvent event(key, 0);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					data.EventCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					//TODO Change 1 to the actual repeating value
					KeyPressedEvent event(key, 1);
					data.EventCallback(event);
					break;
				}
				default:
					break;
			}
		});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, const unsigned int keycode)
		{
			const WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			KeyTypedEvent event(static_cast<int>(keycode));
			data.EventCallback(event);
		});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window,
			const int button, const int action, const int mods)
		{
			const WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			switch (action)
			{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					data.EventCallback(event);
					break;
				}
				default:
					break;
			}
		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, const double xOffset, const double yOffset)
		{
			const WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			
			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.EventCallback(event);
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, const double xPosition, const double yPosition)
		{
			const WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			MouseMovedEvent event(static_cast<float>(xPosition), static_cast<float>(yPosition));
			data.EventCallback(event);
		});
	}

	void WindowsWindow::Shutdown() const
	{
		HZ_PROFILE_FUNCTION()
		
		glfwDestroyWindow(m_Window);
		
		--s_GLFWWindowCount;
		if (s_GLFWWindowCount == 0)
		{
			glfwTerminate();
		}
	}

	void WindowsWindow::OnUpdate()
	{
		HZ_PROFILE_FUNCTION()
		
		glfwPollEvents();
		m_Context->SwapBuffers();
	}

	void WindowsWindow::SetVSync(const bool enabled)
	{
		HZ_PROFILE_FUNCTION()
		
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}
}