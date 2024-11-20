#pragma once

namespace Hazel
{
	enum class ShaderDataType
	{
		None = 0,
		Float, Float2, Float3, Float4,
		Mat3, Mat4,
		Int, Int2, Int3, Int4,
		Bool
	};

	static uint32_t ShaderDataTypeSize(const ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:			return sizeof(float);
			case ShaderDataType::Float2:		return sizeof(float) * 2;
			case ShaderDataType::Float3:		return sizeof(float) * 3;
			case ShaderDataType::Float4:		return sizeof(float) * 4;
			case ShaderDataType::Mat3:			return sizeof(float) * 3 * 3;
			case ShaderDataType::Mat4:			return sizeof(float) * 4 * 4;
			case ShaderDataType::Int:			return sizeof(int);
			case ShaderDataType::Int2:			return sizeof(int) * 2;
			case ShaderDataType::Int3:			return sizeof(int) * 3;
			case ShaderDataType::Int4:			return sizeof(int) * 4;
			case ShaderDataType::Bool:			return sizeof(bool);
			
			case ShaderDataType::None:			HZ_CORE_FATAL("ShaderDataType::None isn't handled.");
		}

		HZ_CORE_ASSERT(false, "Unknown ShaderDataType!")
		return 0;
	}

	struct BufferElement
	{
		std::string Name;
		ShaderDataType Type;
		uint32_t Size;
		size_t Offset;
		bool Normalized;

		BufferElement()
			: Type(ShaderDataType::None), Size(0), Offset(0), Normalized(false) {}

		BufferElement(const ShaderDataType type, std::string name, const bool normalized = false)
			: Name(std::move(name)), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized) {}

		[[nodiscard]] uint32_t GetComponentCount() const
		{
			switch (Type)
			{
				case ShaderDataType::Float:
					return 1;
				case ShaderDataType::Float2:
					return 2;
				case ShaderDataType::Float3:
					return 3;
				case ShaderDataType::Float4:
					return 4;
				case ShaderDataType::Mat3:
					return 3; // 3 * float3
				case ShaderDataType::Mat4:
					return 4; // 4 * float4
				case ShaderDataType::Int:
					return 1;
				case ShaderDataType::Int2:
					return 2;
				case ShaderDataType::Int3:
					return 3;
				case ShaderDataType::Int4:
					return 4;
				case ShaderDataType::Bool:
					return 1;

				case ShaderDataType::None:
					HZ_CORE_FATAL("ShaderDataType::None isn't handled.");
			}

			HZ_CORE_ASSERT(false, "Unknown ShaderDataType!")
			return 0;
		}
	};

	class BufferLayout
	{
	public:
		BufferLayout() : m_Stride(-1) {}

		BufferLayout(const std::initializer_list<BufferElement>& elements)
			: m_Elements(elements), m_Stride(-1)
		{
			CalculateOffsetAndStride();
		}

		[[nodiscard]] const std::vector<BufferElement>& GetElements() const { return m_Elements; }
		[[nodiscard]] uint32_t GetStride() const { return m_Stride; }

		std::vector<BufferElement>::iterator begin()	{ return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end()		{ return m_Elements.end(); }

		[[nodiscard]] std::vector<BufferElement>::const_iterator begin()	const { return m_Elements.begin(); }
		[[nodiscard]] std::vector<BufferElement>::const_iterator end()		const { return m_Elements.end(); }

	private:
		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride;

		void CalculateOffsetAndStride()
		{
			size_t offset = 0;
			m_Stride = 0;
			for (auto& element : m_Elements)
			{
				element.Offset = offset; 
				offset += element.Size;
				m_Stride += element.Size;
			}
		}
	};

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		
		virtual void SetData(const void* data, long long size) = 0;

		[[nodiscard]] virtual const BufferLayout& GetLayout() const = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;

		static Ref<VertexBuffer> Create(uint32_t size);
		static Ref<VertexBuffer> Create(float* vertices, uint32_t size);
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		[[nodiscard]] virtual uint32_t GetCount() const = 0;

		static Ref<IndexBuffer> Create(const uint32_t* indices, uint32_t count);
	};
}