#pragma once

namespace Hazel
{
	class Input
	{
	protected:
		Input() = default;

	public:
		virtual ~Input() = default;
		
		Input(const Input&) = delete;
		
		Input& operator=(const Input&) = delete;
		
		static bool IsKeyPressed(const int keycode)
		{
			return s_Instance->IsKeyPressedImpl(keycode);
		}

		static bool IsMouseButtonPressed(const int button)
		{
			return s_Instance->IsMouseButtonPressedImpl(button);
		}

		static std::pair<float, float> GetMousePosition()
		{
			return s_Instance->GetMousePositionImpl();
		}

		static float GetMouseX()
		{
			return s_Instance->GetMouseXImpl();
		}

		static float GetMouseY()
		{
			return s_Instance->GetMouseYImpl();
		}

	protected:
		virtual bool IsKeyPressedImpl(int keycode) = 0;

		virtual bool IsMouseButtonPressedImpl(int button) = 0;
		virtual std::pair<float, float> GetMousePositionImpl() = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;

	private:
		static Scope<Input> s_Instance;
	};
}