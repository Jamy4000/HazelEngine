workspace "Hazel"
	architecture "x86_64"
	startproject "Hazelnut"

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
IncludeDir["GLFW"] = "HazelEngine/ThirdParties/GLFW/include"
IncludeDir["Glad"] = "HazelEngine/ThirdParties/Glad/include"
IncludeDir["ImGui"] = "HazelEngine/ThirdParties/imgui"
IncludeDir["glm"] = "HazelEngine/ThirdParties/glm"
IncludeDir["stb_image"] = "HazelEngine/ThirdParties/stb_image"
IncludeDir["entt"] = "HazelEngine/ThirdParties/entt/include"

group "Dependencies"
	include "HazelEngine/ThirdParties/GLFW"
	include "HazelEngine/ThirdParties/Glad"
	include "HazelEngine/ThirdParties/imgui"

group ""

project "HazelEngine"
	location "HazelEngine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "hzpch.h"
	pchsource "HazelEngine/src/hzpch.cpp"

	files 
	{ 
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/ThirdParties/stb_image/**.h",
		"%{prj.name}/ThirdParties/stb_image/**.cpp",
		"%{prj.name}/ThirdParties/**.hpp",
		"%{prj.name}/ThirdParties/**.inl"
	}
	
	defines
	{
	    "_CRT_SECURE_NO_WARNINGS",
     	"GLFW_INCLUDE_NONE"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/ThirdParties/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.entt}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib",
		"dwmapi.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
		}

	filter "configurations:Debug"
		defines	"HZ_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "HZ_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "HZ_DIST"
		runtime "Release"
		optimize "on"

	project "Hazelnut"
		location "Hazelnut"
		kind "ConsoleApp"
		language "C++"
		cppdialect "C++17"
		staticruntime "on"
	
		targetdir ("bin/" .. outputdir .. "/%{prj.name}")
		objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	
		files 
		{
			"%{prj.name}/src/**.h",
			"%{prj.name}/src/**.cpp"
		}
	
		includedirs
		{
			"HazelEngine/ThirdParties/spdlog/include",
			"HazelEngine/src",
			"HazelEngine/ThirdParties",
			"%{IncludeDir.glm}",
			"%{IncludeDir.entt}"
		}
	
		links
		{
			"HazelEngine"
		}
	
		filter "system:windows"
			systemversion "latest"
			
		filter "configurations:Debug"
			defines	"HZ_DEBUG"
			runtime "Debug"
			symbols "on"
	
		filter "configurations:Release"
			defines "HZ_RELEASE"
			runtime "Release"
			optimize "on"
	
		filter "configurations:Dist"
			defines "HZ_DIST"
			runtime "Release"
			optimize "on"


project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files 
	{ 
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"HazelEngine/ThirdParties/spdlog/include",
		"HazelEngine/src",
		"HazelEngine/ThirdParties",
		"%{IncludeDir.glm}"
	}

	links
	{
		"HazelEngine"
	}

	filter "system:windows"
		systemversion "latest"
		
	filter "configurations:Debug"
		defines	"HZ_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "HZ_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "HZ_DIST"
		runtime "Release"
		optimize "on"