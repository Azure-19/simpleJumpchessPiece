#include "egpch.h"
#include "OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Engine {

	void OrthographicCamera::RecalculateViewMatrix()
	{
		EG_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *			// always, we need a model matrix but now we havn't
			glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));	// so that's a base 4*4 matrix
		
		m_ViewMatrix = glm::inverse(transform);										// viewMatrix is the inverse of transformMatrix
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;					// because glm is a style of column major
	}

	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		: m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_ViewMatrix(1.0f)
	{
		EG_PROFILE_FUNCTION();

		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;					// a 4*4 identity matrix as defult
	}

	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
	{
		EG_PROFILE_FUNCTION();

		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}