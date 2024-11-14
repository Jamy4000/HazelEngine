#include "hzpch.h"
#include "Renderer.h"

#include "Renderer2D.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Hazel
{
	Scope<Renderer::SceneData> Renderer::s_SceneData = CreateScope<SceneData>();

	void Renderer::Init()
	{
		RenderCommand::Init();
		Renderer2D::Init();
	}

	void Renderer::OnWindowResize(const uint32_t width, const uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(const OrthographicCamera& camera)
	{
		s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
		
	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray,
		const glm::mat4& transform)
	{
		shader->Bind();

		const auto openGLShader = std::dynamic_pointer_cast<OpenGLShader>(shader);
		openGLShader->UploadUniformMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
		openGLShader->UploadUniformMat4("u_Transform", transform);

		vertexArray->Bind();

		RenderCommand::DrawIndexed(vertexArray);
	}
}
