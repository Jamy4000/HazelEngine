#include "hzpch.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

#include "glad/glad.h"

namespace Hazel
{
	// -----------------------------------------------------
	// -------------------Vertex Buffer---------------------
	// -----------------------------------------------------

	OpenGLVertexBuffer::OpenGLVertexBuffer(const float* vertices, const uint32_t size) : m_RendererID(-1)
	{
		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLVertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}

	void OpenGLVertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}


	// -----------------------------------------------------
	// -------------------Index Buffer----------------------
	// -----------------------------------------------------


	OpenGLIndexBuffer::OpenGLIndexBuffer(const uint32_t* indices, const uint32_t count)
		: m_RendererID(-1), m_Count(count)
	{
		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(count * sizeof(uint32_t)), indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLIndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	}

	void OpenGLIndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}