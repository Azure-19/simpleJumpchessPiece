#pragma once

#include "Engine/Renderer/OrthographicCamera.h"
#include "Engine/Renderer/Texture.h"
#include "Engine/Renderer/SubTexture2D.h"

#include "Engine/Renderer/Camera.h"

#include "Engine/Buttons/ButtonAPI.h"

namespace Engine {

	class Renderer2D
	{
	private:
		static void FlushAndReset();

		static std::array<glm::vec2, 2> ScreenCoordProcess(std::array<glm::vec2, 2>& screenCoord);
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const Camera& camera, const glm::mat4& transform);
		static void BeginScene(const OrthographicCamera& camera); // TODO: REMOVE it
		static void EndScene();
		static void Flush();

		// Primitives
		// position is in central, not at origin
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		
		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color);
		static void DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));



		// rotation is in radians
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subtexture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subtexture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		


		static void DrawCoord(const glm::vec2& leftLowerCoord, const glm::vec2& rightHigherCoord, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawCoord(const glm::vec2& leftLowerCoord, const glm::vec2& rightHigherCoord, const Ref<SubTexture2D>& subtexture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		
		template<typename Tex>
		static void DrawButton(Button& button, const Ref<Tex>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f), float scale = 1.0f);
		
		static void DrawSwitchButton(SwitchButton& button, const Ref<Texture2D>& buttonTex, const Ref<Texture2D>& optionTex, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		// with subTexture version
		static void DrawSwitchButton(SwitchButton& button, const Ref<SubTexture2D>& optionTex, const Ref<SubTexture2D>& leftTex, const Ref<SubTexture2D>& rightTex, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));


		// Stats
		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;

			uint32_t GetTotalVertexCount() { return QuadCount * 4; }
			uint32_t GetTotalIndexCount() { return QuadCount * 6; }
		};
		static void ResetStats();
		static Statistics GetStats();

	};

	template<typename Tex>
	inline void Renderer2D::DrawButton(Button& button, const Ref<Tex>& texture, float tilingFactor, const glm::vec4& tintColor, float scale)
	{
		EG_PROFILE_FUNCTION();
		std::array<glm::vec2, 2> data = ScreenCoordProcess(button.GetCoord());
		data[1].x *= scale;
		data[1].y *= scale;

		DrawQuad(data[0], data[1], texture, tilingFactor, tintColor);
	}
}

