#include "egpch.h"
#include "Engine/Core/Application.h"

#include "Engine/Core/Log.h"

#include "Engine/Renderer/Renderer.h"

#include "Engine/Audio/AudioPlayer.h"

#include "Engine/Core/Input.h"

#include <GLFW/glfw3.h>

namespace Engine {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application(const std::string& name)
	{
		EG_PROFILE_FUNCTION();

		EG_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create(WindowProps(name)));	//explicitly call constructor and init GLFW
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));		// to set callback to call onEvent function,so everything we done log some messages
		
		Renderer::Init();
		AudioPlayer::Init();

		m_ImGuiLayer = new ImGuiLayer;							// to set an instance
		PushOverlay(m_ImGuiLayer);

	}	

	Application::~Application()
	{
		EG_PROFILE_FUNCTION();

	}

	void Application::PushLayer(Layer* layer)
	{
		EG_PROFILE_FUNCTION();

		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		EG_PROFILE_FUNCTION();

		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::PopLayer(Layer* layer)
	{
		EG_PROFILE_FUNCTION();

		m_LayerStack.PopLayer(layer);
		layer->OnDetach();
	}

	void Application::PopOverlay(Layer* layer)
	{
		EG_PROFILE_FUNCTION();

		m_LayerStack.PopOverlay(layer);
		layer->OnDetach();
	}

	void Application::EnableLayer(unsigned int index)
	{
		m_LayerStack.EnableLayer(index);
	}

	void Application::DisableLayer(unsigned int index)
	{
		m_LayerStack.DisableLayer(index);
	}

	void Application::ShiftLayer(unsigned int onUseIndex, unsigned int target)
	{
		m_LayerStack.ShiftLayer(onUseIndex, target);
	}

	Layer* Application::VisitLayer(unsigned int index) const
	{
		return m_LayerStack.VisitLayer(index);
	}

	void Application::OnEvent(Event& e)
	{
		EG_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));	//if the event is... then bind... then do 
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));	//if the event is... then bind... then do 

		// Disabled for only supporting first appLayer event
		for (int i = m_LayerStack.GetIndex() - 1; i > -1; i--)			// start from the first real layer(except imgui)
		{
			// disable event if disabled
			if (m_LayerStack[i]->GetEnableState())
				m_LayerStack[i]->OnEvent(e);

			if (e.Handled)
				break;
		}
		
		/* //could not deal with pop event
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )		//reverse
		{
			(*--it)->OnEvent(e);

			if (e.Handled)
				break;
		}*/

	}

	void Application::Run()
	{
		EG_PROFILE_FUNCTION();
		
		while (m_Running)
		{
			EG_PROFILE_SCOPE("Runloop");

			float time = (float)glfwGetTime();			// Platform::GetTime
			Timestep timestep = time - m_LastFrameTime;	// calculate timestep between 2 frame(at least twice)
			m_LastFrameTime = time;

			if (!m_Minimized)
			{
				{
					EG_PROFILE_SCOPE("Layerstack OnUpdate");

					for (Layer* layer : m_LayerStack)
					{
						// Stop Updating it if disabled
						if (layer->GetEnableState())
							layer->OnUpdate(timestep);
					}
				}
				m_ImGuiLayer->Begin();
				{
					EG_PROFILE_SCOPE("Layerstack OnImGuiRender");

					for (Layer* layer : m_LayerStack)
						layer->OnImGuiRender();
				}
				m_ImGuiLayer->End();
			}

			m_Window->OnUpdate();
		}

	}

	void Application::Close()
	{
		m_Running = false;
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		EG_PROFILE_FUNCTION();

		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}

}
