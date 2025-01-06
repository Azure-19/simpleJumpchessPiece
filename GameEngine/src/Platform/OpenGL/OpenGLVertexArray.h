#pragma once

#include "Engine/Renderer/VertexArray.h"

namespace Engine {

	class OpenGLVertexArray : public VertexArray
	{
	private:
		uint32_t m_RendererID;											// keep as a uniform id
		std::vector<Ref<VertexBuffer>> m_VertexBuffers;		// keep reference list of every vertexBuffer put in, mirroring on cpu
		Ref<IndexBuffer> m_IndexBuffer;						// we always need only one
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;							// for a vertexarray unbind is more important

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;

		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const override { return m_VertexBuffers; }
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const override { return m_IndexBuffer; }
	};

}

