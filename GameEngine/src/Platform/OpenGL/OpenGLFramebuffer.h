#pragma once

#include "Engine/Renderer/Framebuffer.h"

namespace Engine {

	// how to define framebuffer is super costomized
	class OpenGLFramebuffer : public Framebuffer
	{
	private:
		uint32_t m_RendererID = 0;
		uint32_t m_ColorAttachment = 0, m_DepthAttachment = 0;
		FramebufferSpecification m_Specification;
	public:
		OpenGLFramebuffer(const FramebufferSpecification& spec);
		virtual ~OpenGLFramebuffer();

		// for the state is invalid and recreate it
		void Invalidate();

		// temporary
		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void Resize(uint32_t width, uint32_t height) override;

		virtual uint32_t GetColorAttachmentRendererID() const override { return m_ColorAttachment; }

		virtual const FramebufferSpecification& GetSpecification() const { return m_Specification; }
	};
}