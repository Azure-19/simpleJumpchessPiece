#pragma once

#include "Engine/Core/Window.h"
#include "Engine/Renderer/GraphicsContext.h"	

#include <GLFW/glfw3.h>

namespace Engine {

	class WindowsWindow : public Window
	{
	private:
		GLFWwindow* m_Window;

		Scope<GraphicsContext> m_Context;

		struct WindowData		//pass it to glfw and also for events INSTEAD OF whole class
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;

	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		virtual void OnUpdate() override;	//update glfw buffer and so on,, work per frame called from application

		inline unsigned int GetWidth() const override	{ return m_Data.Width; }
		inline unsigned int GetHeight() const override	{ return m_Data.Height; }

		// Window attributes
		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		virtual void SetVSync(bool enabled) override;
		virtual bool IsVSync() const override;

		inline virtual void* GetNativeWindow() const override { return m_Window; }
	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
	};
}
