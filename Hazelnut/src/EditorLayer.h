#pragma once

#include <Hazel.h>

namespace Hazel
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		~EditorLayer() override = default; 
	
		void OnAttach() override;
		void OnDetach() override;
	
		void OnUpdate(Timestep ts) override;
	
		void OnImGuiRender() override;
	
		void OnEvent(Event& e) override;
    
	private:
		OrthographicCameraController m_CameraController;

		// TEMP
		Ref<VertexArray> m_SquareVertexArray;
		Ref<Shader> m_FlatColorShader;
		Ref<FrameBuffer> m_FrameBuffer;

		Ref<Texture2D> m_CheckerboardTexture;

		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;
		glm::vec2 m_ViewportSize {0.0f, 0.0f};
	
		glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 0.9f };
	};
}