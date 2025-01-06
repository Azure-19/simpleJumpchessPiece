#pragma once

#include "Engine/Core/Core.h"

namespace Engine{

	struct FramebufferSpecification
	{
		uint32_t Width, Height;
		// FramebufferFormat Format, color config and so on
		uint32_t Samples = 1;

		bool SwapChainTarget = false;
	};

	class Framebuffer
	{
	public:
		// Temporary: you shouldn't bind a framebuffer( it is some kind of virtaul stuff
		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual uint32_t GetColorAttachmentRendererID() const = 0;

		// virtual FramebufferSpecification& GetSpecification() = 0; maybe useful
		virtual const FramebufferSpecification& GetSpecification() const = 0;

		static Ref<Framebuffer> Create(const FramebufferSpecification& spec);
	};
}

