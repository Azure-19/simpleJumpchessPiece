#pragma once

namespace Engine {

	enum class ShaderDataType : uint8_t		// for knowing that we don't neet a 4byte size
	{
		None = 0, Vec1, Vec2, Vec3, Vec4, Mat3, Mat4, Int1, Int2, Int3, Int4, Bool
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Vec1:			return 4;
		case ShaderDataType::Vec2:			return 4 * 2;
		case ShaderDataType::Vec3:			return 4 * 3;
		case ShaderDataType::Vec4:			return 4 * 4;
		case ShaderDataType::Mat3:			return 4 * 3 * 3;
		case ShaderDataType::Mat4:			return 4 * 4 * 4;
		case ShaderDataType::Int1:			return 4;
		case ShaderDataType::Int2:			return 4 * 2;
		case ShaderDataType::Int3:			return 4 * 3;
		case ShaderDataType::Int4:			return 4 * 4;
		case ShaderDataType::Bool:			return 1;
		}

		EG_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0; 
	}

	struct BufferElement
	{
		std::string Name;
		ShaderDataType Type;
		uint32_t Offset;					// to demonstrate the element offset
		uint32_t Size;						// we can get count though it to be honest
		bool Normalized;

		BufferElement() = default;			// redeclare since we have defined a constructor that disabled the default one that list needs

		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
			: Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized)
		{}									// try to set normalized optionally

		uint32_t GetComponentCount() const
		{
			switch (Type)
			{
			case ShaderDataType::None:			EG_CORE_ASSERT(false, "Unknown ShaderDataType!");	return 0;
			case ShaderDataType::Vec1:			return 1;
			case ShaderDataType::Vec2:			return 2;
			case ShaderDataType::Vec3:			return 3;
			case ShaderDataType::Vec4:			return 4;
			case ShaderDataType::Mat3:			return 3 * 3;
			case ShaderDataType::Mat4:			return 4 * 4;
			case ShaderDataType::Int1:			return 1;
			case ShaderDataType::Int2:			return 2;
			case ShaderDataType::Int3:			return 3;
			case ShaderDataType::Int4:			return 4;
			case ShaderDataType::Bool:			return 1;
			}

			EG_CORE_ASSERT(false, "Unknown ShaderDataType!");
			return 0;
		}
	};

	class BufferLayout
	{
	private:
		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride = 0;
	private:
		void CalculateOffsetAndStride()		// go through all the elements
		{
			uint32_t offset = 0;
			m_Stride = 0;
			for (auto& element : m_Elements)// std::vector, std::array, std::list ... ...
			{
				element.Offset = offset;
				offset += element.Size;
				m_Stride += element.Size;
			}
		}
	public:
		BufferLayout() = default;

		BufferLayout(const std::initializer_list<BufferElement>& elements)	// implicitly calling the constructor of BufferElement when initializing
			: m_Elements(elements) 
		{
			CalculateOffsetAndStride();
		}

		inline uint32_t GetStride() const { return m_Stride; }
		inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }

		// we want to use: for (const auto& element : layout){} in application as it's grace
		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }
	};

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;


		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const BufferLayout& GetLayout() const = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;

		virtual void SetData(const void* data, uint32_t size) = 0;

		static Ref<VertexBuffer> Create(uint32_t size); // for batch render
		static Ref<VertexBuffer> Create(float* vertices, uint32_t size);		//real constructors
	};

	// Currently only support 32bit indexBuffers
	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() = default;


		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint32_t GetCount() const = 0;

		static Ref<IndexBuffer> Create(uint32_t* indices, uint32_t count);
	};

}