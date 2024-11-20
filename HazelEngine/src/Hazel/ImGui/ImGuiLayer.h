#pragma once

#include "Hazel/Core/Layer.h"

namespace Hazel
{
	class ImGuiLayer final : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() override = default;

		void Begin();
		void End();

		void OnAttach() override;
		void OnDetach() override;
		
		void OnEvent(Event& e) override;

		void OnImGuiRender() override;

	private:
		float m_Time = 1.0f / 60.0f;
	};
}