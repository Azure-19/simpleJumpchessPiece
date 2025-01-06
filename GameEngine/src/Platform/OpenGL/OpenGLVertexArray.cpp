#include "egpch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Engine {

	// TEMPORARY
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
			case Engine::ShaderDataType::Vec1:				return GL_FLOAT;
			case Engine::ShaderDataType::Vec2:				return GL_FLOAT;
			case Engine::ShaderDataType::Vec3:				return GL_FLOAT;
			case Engine::ShaderDataType::Vec4:				return GL_FLOAT;
			case Engine::ShaderDataType::Mat3:				return GL_FLOAT;
			case Engine::ShaderDataType::Mat4:				return GL_FLOAT;
			case Engine::ShaderDataType::Int1:				return GL_INT;
			case Engine::ShaderDataType::Int2:				return GL_INT;
			case Engine::ShaderDataType::Int3:				return GL_INT;
			case Engine::ShaderDataType::Int4:				return GL_INT;
			case Engine::ShaderDataType::Bool:				return GL_BOOL;
		}

		EG_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		EG_PROFILE_FUNCTION();

		glCreateVertexArrays(1, &m_RendererID);			// so we needn't bind when create it, unless is unbound, some how dangerous
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		EG_PROFILE_FUNCTION();

		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArray::Bind() const
	{
		EG_PROFILE_FUNCTION();

		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		EG_PROFILE_FUNCTION();

		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		// we shoulde call SetLayout func of vertexBuffer outside first
		EG_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex buffer has no layout!");

		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		uint32_t index = 0;
		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.Offset);			// that's fine(if not we couldn't make it run
			index++;
		}

		m_VertexBuffers.push_back(vertexBuffer);		// add it to our vertexbuffer list

		// don't necessary to unbind
	}

	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_RendererID);				// bind vertexarray
		indexBuffer->Bind();							// bind indexbuffer

		m_IndexBuffer = indexBuffer;
	}


}
