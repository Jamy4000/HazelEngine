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

		void BlockEvents(const bool block) { m_BlockEvents = block; }

	private:
		bool m_BlockEvents = true;
		float m_Time = 1.0f / 60.0f;
	};
}