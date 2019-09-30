workspace "LSEngine"
    architecture "x64"
    startproject "Sandbox"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

    IncludeDir = {}
    IncludeDir["GLFW"] = "LSEngine/vendor/glfw/include"
    IncludeDir["GLAD"] = "LSEngine/vendor/glad/include"
    IncludeDir["imgui"] = "LSEngine/vendor/imgui"
    IncludeDir["GLM"] = "LSEngine/vendor/glm"
    IncludeDir["spdlog"] = "LSEngine/vendor/spdlog/include"

    include "LSEngine/vendor/glfw"
    include "LSEngine/vendor/glad"
    include "LSEngine/vendor/imgui"

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
            "LSEngine/src",
            "%{IncludeDir.spdlog}",
            "%{IncludeDir.GLM}",
            "%{IncludeDir.imgui}"
        }
    
        links
        {
            "LSEngine"
        }

        filter "system:windows"
            systemversion "latest"
    
            defines
            {
                "LSE_PLATFORM_WINDOWS",
            }
    
        filter "configurations:Debug"
            defines "LSE_DEBUG"
            runtime "Debug"
            symbols "on"
    
        filter "configurations:Release"
            defines "LSE_RELEASE"
            runtime "Release"
            optimize "on"
    
        filter "configurations:Dist"
            defines "LSE_DIST"
            runtime "Release"
            optimize "on"

    project "LSEngine"
        location "LSEngine"
        kind "StaticLib"
        language "C++"
        cppdialect "C++17"
        staticruntime "on"

        targetdir ("bin/" .. outputdir .. "/%{prj.name}")
        objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
        
        pchheader "lsepch.h"
        pchsource "LSEngine/src/lsepch.cpp"

        files
        {
            "%{prj.name}/src/**.h",
            "%{prj.name}/src/**.cpp"
        }
        
        includedirs
        {
            "%{prj.name}/vendor/spdlog/include",
            "%{prj.name}/vendor/glm",
            "%{prj.name}/src",
            "%{IncludeDir.GLFW}",
            "%{IncludeDir.GLAD}",
            "%{IncludeDir.imgui}",
            "%{IncludeDir.GLM}"
        }
        
        links
        {
            "GLFW",
            "GLAD",
            "imgui",
            "opengl32.lib"
        }

        defines
        {
            "_CRT_SECURE_NO_WARNINGS"
        }

        filter "system:windows"
            systemversion "latest"
        
            defines
            {
                "IMGUI_IMPL_OPENGL_LOADER_GLAD",
                "GLFW_INCLUDE_NONE",
                "LSE_PLATFORM_WINDOWS",
                "LSE_BUILD_DLL"
            }

        filter "configurations:Debug"
            defines
            {
                "LSE_DEBUG",
                "LSE_ENABLE_ASSERTS"
            }
            runtime "Debug"
            symbols "on"
    
        filter "configurations:Release"
            defines "LSE_RELEASE"
            runtime "Release"
            optimize "on"
    
        filter "configurations:Dist"
            defines "LSE_DIST"
            runtime "Release"
            optimize "on"