#pragma once

#include "Engine/Renderer/RendererAPI.h"

namespace Engine {

	class RenderCommand
	{
	private:
		static Scope<RendererAPI> s_RendererAPI; // point to a class without members(only one byte)
	public:
		inline static void Init()
		{
			s_RendererAPI->Init();
		}

		inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			s_RendererAPI->SetViewport(x, y, width, height);
		}

		inline static void SetClearColor(const glm::vec4& color)
		{
			s_RendererAPI->SetClearColor(color);
		}
		inline static void Clear()
		{
			s_RendererAPI->Clear();
		}
		// 0 by default for only one quad
		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0)
		{
			s_RendererAPI->DrawIndexed(vertexArray, indexCount);// dispatch it into RenderCommand class
		}

		inline static void DrawCovered(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0)
		{
			s_RendererAPI->DrawCovered(vertexArray, indexCount);
		}
	};

}