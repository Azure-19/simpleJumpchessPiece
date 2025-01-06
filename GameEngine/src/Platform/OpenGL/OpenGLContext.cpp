#include "egpch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <Glad/glad.h>
#include <gl/GL.h>

namespace Engine {

    OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
        : m_WindowHandle(windowHandle)
    {
        EG_CORE_ASSERT(windowHandle, "Window handle is null!");
    }

    void OpenGLContext::Init()
    {
        EG_PROFILE_FUNCTION();

        glfwMakeContextCurrent(m_WindowHandle);
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        EG_CORE_ASSERT(status, "Failed to initialize Glad!");

        EG_CORE_INFO(" OpenGL Info: ");
        EG_CORE_INFO(" Vendor: {0}",    (const char*)glGetString(GL_VENDOR));
        EG_CORE_INFO(" Renderer: {0}",  (const char*)glGetString(GL_RENDERER));
        EG_CORE_INFO(" Version: {0}",   (const char*)glGetString(GL_VERSION));

    #ifdef EG_ENABLE_ASSERTS
        int versionMajor;
        int versionMinor;
        glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
        glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

        EG_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), "Require at least OpenGL version 4.5!");
    #endif
    }
    void OpenGLContext::SwapBuffers()
    {
        EG_PROFILE_FUNCTION();

        glfwSwapBuffers(m_WindowHandle);
    }

}