#pragma once

#include "Hazel/Core/Input.h"

namespace Hazel
{
	class WindowsInput final : public Input
	{
	protected:
		bool IsKeyPressedImpl(KeyCode keycode) override;

		bool IsMouseButtonPressedImpl(MouseCode button) override;
		std::pair<float, float> GetMousePositionImpl() override;
		float GetMouseXImpl() override;
		float GetMouseYImpl() override;
	};
}