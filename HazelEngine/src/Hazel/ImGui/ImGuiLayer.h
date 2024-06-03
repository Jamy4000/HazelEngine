#pragma once

#include "Hazel/Layer.h"
#include "Hazel/Events/KeyEvent.h"
#include "Hazel/Events/MouseEvent.h"
#include "Hazel/Events/ApplicationEvent.h"

namespace Hazel
{
	class  ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		virtual ~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnImGuiRender() override;

		virtual void Begin();
		virtual void End();

	private:
		float m_Time = 1.0f / 60.0f;
	};
}