#pragma once

#include "Engine/Renderer/Camera.h"

namespace Engine {

	class SceneCamera : public Camera
	{
	private:
		// calculate by aspectRatio later
		float m_OrthoGraphicSize = 10.0f;
		float m_OrthoGraphicNear = -1.0f, m_OrthoGraphicFar = 1.0f;
		float m_AspectRatio = 16.0f / 9.0f;
	private:
		void RecalculateProjection();
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		void SetOrthoGraphic(float size, float nearClip, float farClip);

		void SetViewportSize(uint32_t width, uint32_t height);
	};
}