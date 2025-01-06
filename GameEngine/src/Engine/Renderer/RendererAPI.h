#pragma once

#include <glm/glm.hpp>

#include "Engine/Renderer/VertexArray.h"

namespace Engine {

	class RendererAPI
	{
	public:
		enum class API
		{
			None = 0, OpenGL = 1
		};
	private:
		static API s_API;		// save as static variable

	public:						// only give commands so const doesn't a matter
		virtual void Init() = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		// we can't call drawIndexed without indexCount actually
		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) = 0;
		// TEMPORARY: used for those render upper than background(especially with alpha = 0 area)
		// -[WARNING]- LOST synchronization with drawIndexed
		virtual void DrawCovered(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) = 0;

		inline static API GetAPI() { return s_API; }
		static Scope<RendererAPI> Create();
	};
}