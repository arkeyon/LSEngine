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
            "LSEngine/vendor/spdlog/include",
            "LSEngine/vendor/glm",
            "LSEngine/src"
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
            symbols "On"
    
        filter "configurations:Release"
            defines "LSE_RELEASE"
            optimize "On"
    
        filter "configurations:Dist"
            defines "LSE_DIST"
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
            "%{prj.name}/src"
        }
        
        filter "system:windows"
            cppdialect "C++17"
            staticruntime "On"
            systemversion "latest"
        
            defines
            {
                "LSE_PLATFORM_WINDOWS",
                "LSE_BUILD_DLL"
            }
        
            postbuildcommands
            {
                ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
            }
        
        filter "configurations:Debug"
            defines "LSE_DEBUG"
            symbols "On"
        
        filter "configurations:Release"
            defines "LSE_RELEASE"
            optimize "On"
        
        filter "configurations:Dist"
            defines "LSE_DIST"
            optimize "On"