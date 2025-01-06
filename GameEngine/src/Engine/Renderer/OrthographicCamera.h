#pragma once

#include <glm/glm.hpp>

namespace Engine {

	class OrthographicCamera						// mainly 2D stuff
	{
	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;						// we want to recalculate it per frame/? equals transform matrix inverted
		glm::mat4 m_ViewProjectionMatrix;			// cache the multiplation with 64 extra bytes spent(or VP

		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };// for 2D we disabled z
		float m_Rotation = 0.0f;					// for a 2D we always just want to rotate it by z
	private:
		void RecalculateViewMatrix();				// for updating some
	public:
		OrthographicCamera(float left, float right, float bottom, float top);		// not a matrix but a bound
		// does the same constructor does
		void SetProjection(float left, float right, float bottom, float top);


		const glm::vec3& GetPosition() const			{ return m_Position; }
		void SetPosition(const glm::vec3& position)		 { m_Position = position; RecalculateViewMatrix(); }

		float GetRotation() const						{ return m_Rotation; }
		void SetRotation(float rotation)				 { m_Rotation = rotation; RecalculateViewMatrix(); }


		const glm::mat4& GetProjectionMatrix() const		{ return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const				{ return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const	{ return m_ViewProjectionMatrix; }
	};

}