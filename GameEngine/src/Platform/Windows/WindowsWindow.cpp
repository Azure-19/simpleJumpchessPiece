#include "egpch.h"
#include "WindowsWindow.h"

#include "Engine/Events/KeyEvent.h"
#include "Engine/Events/MouseEvent.h"
#include "Engine/Events/GamepadEvent.h"
#include "Engine/Events/ApplicationEvent.h"

#include "Platform/OpenGL/OpenGLContext.h"

namespace Engine {

	static uint8_t s_GLFWWindowCount = 0;

	static void GLFWErrorCallback(int error, const char* description)
	{
		EG_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	Scope<Window> Window::Create(const WindowProps& props)	//because of Window has an virtual Create
	{
		return CreateScope<WindowsWindow>(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		EG_PROFILE_FUNCTION();

		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		EG_PROFILE_FUNCTION();

		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		EG_PROFILE_FUNCTION();

		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		EG_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);


		if (!s_GLFWWindowCount)	//we want to initialize it once and maybe need more than one window
		{
			EG_PROFILE_SCOPE("glfwInit");

			// TODO: glfwTerminate on system shutdown
			int success = glfwInit();
			EG_CORE_ASSERT(success, "Could not intialize GLFW!");		//disabled in release version
			glfwSetErrorCallback(GLFWErrorCallback);
			++s_GLFWWindowCount;
		}


		{
			EG_PROFILE_SCOPE("glfwCreateWindow");

			m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
			m_Context = CreateScope<OpenGLContext>(m_Window);
		}


		m_Context->Init();

		glfwSetWindowUserPointer(m_Window, &m_Data);	//as if binding a databuffer and a pointer(seen in opengl)
		SetVSync(true);

		// Set GLFW callbacks
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
			{	//using a lambda here, to see the definition of GLFWwindow since the gives a void*, we can assess data sturcture
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				//do refresh first
				data.Width = width;
				data.Height = height;

				WindowResizeEvent event(width, height);
				data.EventCallback(event);
			});			//the whole body is a lambda actually, the very first function is a glfw func

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				WindowCloseEvent event;
				data.EventCallback(event);
			});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
					case GLFW_PRESS:
					{
						KeyPressedEvent event(key, 0);
						data.EventCallback(event);
						break;
					}
					case GLFW_RELEASE:
					{
						KeyReleasedEvent event(key);
						data.EventCallback(event);
						break;
					}
					case GLFW_REPEAT:
					{
						KeyPressedEvent event(key, 1);
						data.EventCallback(event);
						break;
					}
				}
			});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				KeyTypedEvent event(keycode);
				data.EventCallback(event);
			});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
					case GLFW_PRESS:
					{
						MouseButtonPressedEvent event(button);
						data.EventCallback(event);
						break;
					}
					case GLFW_RELEASE:
					{
						MouseButtonReleasedEvent event(button);
						data.EventCallback(event);
						break;
					}
				}

			});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseScrolledEvent event((float)xOffset, (float)yOffset);		//type cast
				data.EventCallback(event);
			});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseMovedEvent event((float)xPos, (float)yPos);		//type cast
				data.EventCallback(event);
			});
		// -TODO: figure out a way passing info back into EventFN
		glfwSetJoystickCallback([](int joystickID, int state) 
			{
				if (state == GLFW_CONNECTED)
				{
					EG_CORE_INFO("Joystick Connected. ID: {0}.", joystickID);
				}
				else if (state == GLFW_DISCONNECTED)
				{
					EG_CORE_INFO("Joystick Disconnected. ID:{0}", joystickID);
				}
			});
	}

	void WindowsWindow::Shutdown()
	{
		EG_PROFILE_FUNCTION();

		glfwDestroyWindow(m_Window);
		--s_GLFWWindowCount;

		if (s_GLFWWindowCount == 0)
		{
			glfwTerminate();
		}
	}

	void WindowsWindow::OnUpdate()
	{
		EG_PROFILE_FUNCTION();

		glfwPollEvents();
		m_Context->SwapBuffers();
	}

	void WindowsWindow::SetVSync(bool enabled)	//now implcated by glfw just
	{
		EG_PROFILE_FUNCTION();

		if (enabled)
			glfwSwapInterval(1);		//up to N times(depends on the monitor fps)
		else
			glfwSwapInterval(0);

		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}

}