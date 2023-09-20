workspace "2DVulkan"
	architecture "x64"
	
	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "2DVulkan/vender/GLFW/include"
IncludeDir["glm"] = "2DVulkan/vender/glm/include"
IncludeDir["stb_image"] = "2DVulkan/vender/stb_image/include"
IncludeDir["vulkan"] = "2DVulkan/vender/vulkan/Include"
IncludeDir["stb_truetype"] = "2DVulkan/vender/stb_truetype/include"
IncludeDir["freetype"] = "2DVulkan/vender/freetype/include"

include "2DVulkan/vender/GLFW"

project "2DVulkan"
	location "2DVulkan"
	kind "ConsoleApp"
	-- clr "On"
	language "C++"
	cppdialect "C++17"
	
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vender/stb_image/**.h",
		"%{prj.name}/vender/vender/freetype/freetype/include/**.h",
		
		"%{prj.name}/vender/glm/glm/**.hpp",
		"%{prj.name}/vender/glm/glm/**.inl",
		"%{prj.name}/vender/stb_image/**.h",
		"%{prj.name}/vender/vulkan/vulkan.h",
		"%{prj.name}/vender/freetype/ft2build.h",

		"%{prj.name}/vender/imgui/imconfig.h",
		"%{prj.name}/vender/imgui/imgui.h",
		"%{prj.name}/vender/imgui/imgui.cpp",
		"%{prj.name}/vender/imgui/imgui_draw.cpp",
		"%{prj.name}/vender/imgui/imgui_internal.h",
		"%{prj.name}/vender/imgui/imgui_tables.cpp",
		"%{prj.name}/vender/imgui/imgui_widgets.cpp",
		"%{prj.name}/vender/imgui/imstb_rectpack.h",
		"%{prj.name}/vender/imgui/imstb_textedit.h",
		"%{prj.name}/vender/imgui/imstb_truetype.h",
		"%{prj.name}/vender/imgui/imgui_demo.cpp",
		"%{prj.name}/vender/imgui/backends/imgui_impl_glfw.h",
		"%{prj.name}/vender/imgui/backends/imgui_impl_glfw.cpp",
		"%{prj.name}/vender/imgui/backends/imgui_impl_vulkan.h",
		"%{prj.name}/vender/imgui/backends/imgui_impl_vulkan.cpp"
	}
	
	includedirs
	{
		"%{prj.name}/src",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.vulkan}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.stb_truetype}",
		"%{IncludeDir.freetype}",
		"%{prj.name}/vender/imgui"
	}

	links
	{
		"GLFW",
		"2DVulkan/vender/vulkan/Lib/vulkan-1.lib",
		"2DVulkan/vender/freetype/lib/freetype.lib"
	}
	
	ABSOLUTE_PATH = path.getabsolute(os.getcwd())
	
	defines
	{
	'RESOURCES="'..ABSOLUTE_PATH..'/Clever/Clever/Resources/"'
	}
	
	filter "system:windows"
		
		staticruntime "On"
		systemversion "latest"
		
	filter "configurations:Debug"
		staticruntime "off"
		runtime "Debug"
		
	filter "configurations:Release"
		staticruntime "off"
		runtime "Release"