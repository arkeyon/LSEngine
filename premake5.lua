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
            "%{IncludeDir.GLM}"
        }
    
        links
        {
            "LSEngine"
        }

        filter "system:windows"
            cppdialect "C++17"
            staticruntime "On"
            systemversion "latest"
    
            defines
            {
                "LSE_PLATFORM_WINDOWS",
            }
    
        filter "configurations:Debug"
            defines "LSE_DEBUG"
            buildoptions "/MDd"
            symbols "On"
    
        filter "configurations:Release"
            defines "LSE_RELEASE"
            buildoptions "/MD"
            optimize "On"
    
        filter "configurations:Dist"
            defines "LSE_DIST"
            buildoptions "/MD"
            optimize "On"

    project "LSEngine"
        location "LSEngine"
        kind "SharedLib"
        language "C++"
        
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

        filter "system:windows"
            cppdialect "C++17"
            staticruntime "On"
            systemversion "latest"
        
            defines
            {
                "IMGUI_IMPL_OPENGL_LOADER_GLAD",
                "GLFW_INCLUDE_NONE",
                "LSE_PLATFORM_WINDOWS",
                "LSE_BUILD_DLL"
            }
        
            postbuildcommands
            {
                ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
            }
        
        filter "configurations:Debug"
            defines
            {
                "LSE_DEBUG",
                "LSE_ENABLE_ASSERTS"
            }
            buildoptions "/MDd"
            symbols "On"
        
        filter "configurations:Release"
            defines "LSE_RELEASE"
            buildoptions "/MD"
            optimize "On"
        
        filter "configurations:Dist"
            defines "LSE_DIST"
            buildoptions "/MD"
            optimize "On"