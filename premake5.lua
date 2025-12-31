workspace "HydrogenViewer"
    configurations { "Debug", "Release" }
    platforms {"x64"}

	OutputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

    filter { "platforms:x64" }
        system "Windows"
        architecture "x86_64"

    filter {"configurations:Debug"}
        defines {"DEBUG"}
        optimize "off"

    filter {"configurations:Release"}
        defines {"RELEASE"}
        optimize "on"

        startproject "HydrogenViewer"

    project "HydrogenViewer"
        location "./HydrogenViewer"
        kind "ConsoleApp"
        language "C++"

        targetdir ("./bin/"..OutputDir.."/PlayGround")
        objdir ("./bin-obj/"..OutputDir.."/PlayGround")


        filter ("system:windows")
			defines {"WINDOWS"}


        defines {"GLEW_STATIC"}

        files {
            "./HydrogenViewer/**.cpp",
            "./HydrogenViewer/**.h"
        }

        includedirs{
            "./3rdParty/glfw/include/",
            "./3rdParty/imgui",
            "./HydrogenViewer/",
            "./Hydrogen/"

        }

        libdirs{
			"./bin/"..OutputDir.."/glfw",
			"./bin/"..OutputDir.."/imgui",
            "./bin/"..OutputDir.."/Hydrogen",
            "./Hydrogen/Source/Glew"
        }

        links {"Hydrogen","opengl32", "glew32s","imgui", "glfw","kernel32", "user32" , "gdi32", "winspool", "comdlg32", "advapi32", "shell32","ole32", 
		"oleaut32", "uuid", "odbc32", "odbccp32"}


    project "Hydrogen"
        location "./Hydrogen"
        kind "StaticLib"
        language "C++"


        targetdir ("./bin/"..OutputDir.."/Hydrogen")
        objdir ("./bin-obj/"..OutputDir.."/Hydrogen")
        files{
            "./Hydrogen/**.cpp",
            "./Hydrogen/**.h"
        }
        
        defines {"GLEW_STATIC"}

        filter {"configurations:Debug"}
            defines {"HYD_DEBUG"}
            optimize "off"

        filter {"configurations:Release"}
            defines {"HYD_RELEASE"}
            optimize "on"
        filter ("system:windows")
			defines {"HYD_PLATFORM_WINDOWS"}

        includedirs{
            "./Hydrogen"
        }
        libdirs{
            "./Hydrogen/Source/Glew"
        }
        links {"opengl32", "glew32s"}

    project "glfw"
        location "./3rdParty/glfw"
        kind "SharedLib"
		language "C"

        targetdir ("./bin/"..OutputDir.."/glfw")
        objdir ("./bin-obj/"..OutputDir.."/glfw")


        files {
            "./3rdParty/glfw/src/**.c",
            "./3rdParty/glfw/include/**.h"
        }

        filter ("system:windows")
			defines {"_WIN32", "_GLFW_WIN32"}
			systemversion "10.0.17763.0"
			staticruntime "On"

		filter("configurations:Debug")
			symbols "On"
			defines {"_GLFW_BUILD_DLL"}
		filter("configurations:Release")
			optimize "On"
			defines {"_GLFW_BUILD_DLL"}

		filter {"system:windows", "configurations:Debug"}
			buildoptions {"/MDd"}
		filter {"system:windows", "configurations:Release"}
			buildoptions {"/MT"}


    project "ImGui"
       location "./3rdParty/Imgui"
        kind "StaticLib"
		language "C++"

        targetdir ("./bin/"..OutputDir.."/Imgui")
        objdir ("./bin-obj/"..OutputDir.."/imgui")


        files {
            "./3rdParty/imgui/**.cpp",
            "./3rdParty/imgui/**.h"
        }

        includedirs{
            "./3rdParty/glfw/include/"
        }

		filter("configurations:Debug")
			symbols "On"

		filter("configurations:Release")
			optimize "On"


		filter {"system:windows", "configurations:Debug"}
			buildoptions {"/MDd"}
		filter {"system:windows", "configurations:Release"}
			buildoptions {"/MT"}