#pragma once

#include "hzpch.h"

#include "Hazel/Events/Event.h"

namespace Hazel
{
	struct WindowProps
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		explicit WindowProps(std::string title = "Hazel Engine",
		                     const unsigned int width = 1280,
		                     const unsigned int height = 720)
			: Title(std::move(title)), Width(width), Height(height) {}
	};

	// interface representing a desktop system based Window
	class Window
	{
	public:
		// methods that returns void and takes an Event as parameter
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() = default;

		virtual void OnUpdate() = 0;

		[[nodiscard]] virtual unsigned int GetWidth() const = 0;
		[[nodiscard]] virtual unsigned int GetHeight() const = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		[[nodiscard]] virtual bool IsVSync() const = 0;

		[[nodiscard]] virtual void* GetNativeWindow() const = 0;

		static Scope<Window> Create(const WindowProps& props = WindowProps());
	};
}