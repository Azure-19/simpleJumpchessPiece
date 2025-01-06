#include "egpch.h"
#include "SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Engine {

	void SceneCamera::RecalculateProjection()
	{
		EG_CORE_ASSERT(m_AspectRatio, "AspectRatio must be non-zero");
		float orthoLeft = -m_OrthoGraphicSize * m_AspectRatio * 0.5f;
		float orthoRight = m_OrthoGraphicSize * m_AspectRatio * 0.5f;
		float orthoBottom = -m_OrthoGraphicSize * 0.5f;
		float orthoTop = m_OrthoGraphicSize * 0.5f;

		m_Projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop,
			m_OrthoGraphicNear, m_OrthoGraphicFar);
	}

	SceneCamera::SceneCamera()
	{
		RecalculateProjection();
	}

	void SceneCamera::SetOrthoGraphic(float size, float nearClip, float farClip)
	{
		m_OrthoGraphicSize = size;
		m_OrthoGraphicNear = nearClip;
		m_OrthoGraphicFar = farClip;

		RecalculateProjection();

	}

	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		EG_CORE_ASSERT(width & height, "AspectRatio must be non-zero");
		m_AspectRatio = (float)width / (float)height;

		RecalculateProjection();
	}

}