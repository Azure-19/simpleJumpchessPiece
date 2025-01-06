#pragma once

#include "Engine/Renderer/OrthographicCamera.h"
#include "Engine/Core/Timestep.h"

#include "Engine/Events/ApplicationEvent.h"
#include "Engine/Events/MouseEvent.h"

namespace Engine {

	struct OrthographicCameraBounds
	{
		float Left, Right;
		float Bottom, Top;

		float GetWidth() { return Right - Left; }
		float GetHeight() { return Top - Bottom; }
	};

	class OrthographicCameraController
	{
	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;								// our scale
		OrthographicCameraBounds m_Bounds;
		OrthographicCamera m_Camera;							// initialize last because of dependences

		bool m_Rotation;

		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraRotation = 0.0f; // by degrees (check it out by searching "glm::radians(m_Rotation)"
		float m_CameraTranslationSpeed = 5.0f, m_CameraRotationSpeed = 90.0f;
	private:
		void CalculateView();

		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);

	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);		// by default: (16:9 or ...) * 2units

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		void OnResize(float width, float height);

		OrthographicCamera& GetCamera()				 { return m_Camera; }
		const OrthographicCamera& GetCamera() const	 { return m_Camera; }

		float GetZoomLevel() { return m_ZoomLevel; }
		// in case you want to show more stuff instead of bigger stuff when window resized;  maybe useless now
		void SetZoomLevel(float level) { m_ZoomLevel = level; CalculateView(); }

		const OrthographicCameraBounds& GetBounds() const { return m_Bounds; }
	};
}