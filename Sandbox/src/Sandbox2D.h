#pragma once

#include <Hazel.h>

#include "ParticleSystem.h"

class Sandbox2D : public Hazel::Layer
{
public:
	Sandbox2D();
	~Sandbox2D() override = default; 
	
	void OnAttach() override;
	void OnDetach() override;
	
	void OnUpdate(Hazel::Timestep ts) override;
	
	void OnImGuiRender() override;
	
	void OnEvent(Hazel::Event& e) override;
    
private:
	Hazel::OrthographicCameraController m_CameraController;

	// TEMP
	Hazel::Ref<Hazel::VertexArray> m_SquareVertexArray;
	Hazel::Ref<Hazel::Shader> m_FlatColorShader;

	Hazel::Ref<Hazel::Texture2D> m_CheckerboardTexture;
	
	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 0.9f };
};