#pragma once

#include "Engine/Renderer/RenderCommand.h"

#include "Engine/Renderer/OrthographicCamera.h"
#include "Engine/Renderer/Shader.h"

namespace Engine {

	class Renderer
	{
	// TEMPORARY
	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* s_SceneData;
	public:
		static void Init();
		static void OnWindowResize(uint32_t width, uint32_t height);

		static void BeginScene(OrthographicCamera& camera);							// to receive scene parameters we need
		static void EndScene();
		// submit our render call by giving a actual shader, identity matrix by default
		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray,
		const glm::mat4& transform = glm::mat4(1.0f));

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI() ; }	// need only one this func
	};

}