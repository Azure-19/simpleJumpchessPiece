#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Core/Window.h"
#include "Engine/Core/LayerStack.h"
#include "Engine/Events/Event.h"
#include "Engine/Events/ApplicationEvent.h"

#include "Engine/Core/Timestep.h"

#include "Engine/ImGui/ImGuiLayer.h"

namespace Engine {

	class Application
	{
	private:
		std::unique_ptr<Window> m_Window;				// for automate its delete operation
		ImGuiLayer* m_ImGuiLayer;						// to set an ImGuiLayer
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;						// on stack
		float m_LastFrameTime;							// track frameTime
	private:											// using shared_ptr because we need the count
		static Application* s_Instance;
	private:
		bool OnWindowClose(WindowCloseEvent& e);		// a close event
		bool OnWindowResize(WindowResizeEvent& e);		// resize event changing randering area
	public:
		Application(const std::string& name = "Azur App");
		virtual ~Application();

		void Run();

		void Close();

		// Need specific eventCallbacks in subclass
		void OnEvent(Event& e);							// func deal with event

		// for client app(shouldn't used in button events)s
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* layer);

		void EnableLayer(unsigned int index);
		void DisableLayer(unsigned int index);
		void ShiftLayer(unsigned int onUseIndex, unsigned int target);
		Layer* VisitLayer(unsigned int index) const;


		inline Window& GetWindow() { return *m_Window; }

		// Give the control out to Client
		inline static Application& Get() { return *s_Instance; }
	};

	// To be defined in a CLIENT
	Application* CreateApplication();

}

