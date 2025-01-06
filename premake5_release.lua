workspace "GameEngine"
	architecture "x86_64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "GameEngine/vendor/GLFW/include"
IncludeDir["Glad"] = "GameEngine/vendor/Glad/include"
IncludeDir["ImGui"] = "GameEngine/vendor/imgui"
IncludeDir["glm"] = "GameEngine/vendor/glm"
IncludeDir["stb_image"] = "GameEngine/vendor/stb_image"
IncludeDir["entt"] = "GameEngine/vendor/entt/include"
IncludeDir["OpenAL"] = "GameEngine/vendor/OpenAL/include"


-- Lib directories relative to root folder
LibDir = {}
LibDir["OpenAL"] = "GameEngine/vendor/OpenAL/libs/Win64"


include "GameEngine/vendor/GLFW"
include "GameEngine/vendor/Glad"
include "GameEngine/vendor/imgui"

project "GameEngine"
	location "GameEngine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "egpch.h"
	pchsource "GameEngine/src/egpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}

	includedirs
	{
		"GameEngine/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.OpenAL}"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
	}

	libdirs {
        "%{LibDir.OpenAL}",
    }

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib",
		"OpenAL32.lib"
	}
	
	filter "system:windows"
		systemversion "latest"
		buildoptions { "/wd26498" }

		defines
		{
			"EG_PLATFORM_WINDOWS",
			"EG_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines "EG_DEBUG"
		runtime "Debug"
		symbols "on"	

	filter "configurations:Release"
		defines "EG_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "EG_DIST"
		runtime "Release"
		optimize "on"



project "Sandbox"
	location "Sandbox"
	kind "WindowedApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	includedirs
	{
		"GameEngine/vendor/spdlog/include",
		"GameEngine/src",
		"GameEngine/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
		"Sandbox/src",
		"%{IncludeDir.OpenAL}"
	}

	links
	{
		"GameEngine"
	}

	linkoptions
	{
		"/ENTRY:mainCRTStartup",
        "/SUBSYSTEM:WINDOWS"
	}
	filter "system:windows"
		systemversion "latest"
		buildoptions { "/wd26498" }

		defines
		{
			"EG_PLATFORM_WINDOWS",
		}

		filter "configurations:Debug"
		defines "EG_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "EG_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "EG_DIST"
		runtime "Release"
		optimize "on"