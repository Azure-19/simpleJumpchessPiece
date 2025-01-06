#pragma once

#include "Engine/Renderer/GraphicsContext.h"

struct GLFWwindow;		//declaration because don't want to include

namespace Engine {

	class OpenGLContext : public GraphicsContext
	{
	private:
		GLFWwindow* m_WindowHandle;
	public:
		OpenGLContext(GLFWwindow* windowHandle);


		virtual void Init() override;
		virtual void SwapBuffers() override;
	};

}