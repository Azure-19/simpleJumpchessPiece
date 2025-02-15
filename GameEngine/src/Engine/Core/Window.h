#pragma once

#include "egpch.h"

#include "Engine/Core/Core.h"
#include "Engine/Events/Event.h"

namespace Engine {

	struct WindowProps		// some kind of data struct
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;

		WindowProps(const std::string& title = "Azur_",
			uint32_t width = 1280,
			uint32_t height = 720)		//if not specified ,then default
			: Title(title), Width(width), Height(height)
		{
		}
	};

	// Interface representing a desktop system based Window
	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;		//callback function

		virtual ~Window() = default;

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0; //to provide a modifiable pointer(for multi-platform)

		static Scope<Window> Create(const WindowProps& props = WindowProps());	//must be implemented per platform, auto when compiling
	};

}