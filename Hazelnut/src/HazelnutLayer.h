#pragma once

#include <Hazel.h>

namespace Hazel
{
	class HazelnutLayer : public Layer
	{
	public:
		HazelnutLayer();
		~HazelnutLayer() override = default; 
	
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
	
		glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 0.9f };
	};
}