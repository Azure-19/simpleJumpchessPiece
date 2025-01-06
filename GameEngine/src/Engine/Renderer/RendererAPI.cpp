#include "egpch.h"
#include "RendererAPI.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Engine {

	RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

	Scope<RendererAPI> RendererAPI::Create()
	{
		switch (s_API)
		{
		case Engine::RendererAPI::API::None:	EG_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case Engine::RendererAPI::API::OpenGL:	return CreateScope<OpenGLRendererAPI>();
		}
		EG_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}