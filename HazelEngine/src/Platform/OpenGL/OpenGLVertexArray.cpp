#include "hzpch.h"
#include "OpenGLVertexArray.h"

#include "glad/glad.h"

namespace Hazel
{
	static GLenum ShaderDataTypeToOpenGLBaseType(const ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:
			case ShaderDataType::Float2:
			case ShaderDataType::Float3:
			case ShaderDataType::Float4:
			case ShaderDataType::Mat3:
			case ShaderDataType::Mat4:
				return GL_FLOAT;
			
			case ShaderDataType::Int:
			case ShaderDataType::Int2:
			case ShaderDataType::Int3:
			case ShaderDataType::Int4:
				return GL_INT;
			
			case ShaderDataType::Bool:
				return GL_BOOL;

			case ShaderDataType::None:
				HZ_CORE_ASSERT(false, "None ShaderDataType!")
		}

		HZ_CORE_ASSERT(false, "Unknown ShaderDataType!")
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray() : m_RendererID(0)
	{
		glCreateVertexArrays(1, &m_RendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& buffer)
	{
		HZ_CORE_ASSERT(buffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!")

		glBindVertexArray(m_RendererID);
		buffer->Bind();

		const auto& layout = buffer->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(m_VertexBufferIndex);
			
			glVertexAttribPointer(m_VertexBufferIndex,
				static_cast<int>(element.GetComponentCount()),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				static_cast<int>(layout.GetStride()),
				reinterpret_cast<const void*>(element.Offset));
			
			m_VertexBufferIndex++;
		}

		m_VertexBuffers.push_back(buffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& buffer)
	{
		glBindVertexArray(m_RendererID);
		buffer->Bind();

		m_IndexBuffers = buffer;
	}
}