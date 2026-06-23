project "Hydrogen"
    kind "SharedLib"
    language "C++"
    cppdialect "C++17"
    location "./Engine/"
    targetdir ("%{wks.location}/bin/"..OutputDir.."/Hydrogen")
    objdir ("%{wks.location}/bin-obj/"..OutputDir.."/Hydrogen")

    files{
        "./Engine/**.cpp",
        "./Engine/**.h",
        "Hydrogen.h"
    }
     
    defines {"GLEW_STATIC", "HYD_DLL_BUILD"}

    filter {"configurations:Debug"}
        defines {"HYD_DEBUG"}
        optimize "off"
    filter {"configurations:Release"}
        defines {"HYD_RELEASE"}
        optimize "on"
    filter {"configurations:Dist"}
        defines {"HYD_DIST"}
        optimize "on"

    filter ("system:windows")
		defines {"HYD_PLATFORM_WINDOWS"}
    
    flags { "NoPCH" }

	pchheader "HydPch.h"
	pchsource "./Source/HydPch.cpp"

    includedirs{
        "%{wks.location}/Hydrogen/Engine/",
        "%{wks.location}/Hydrogen/3rdParty",
        "%{wks.location}/Hydrogen/3rdParty/glfw/include",
        "%{wks.location}/Hydrogen/3rdParty/imgui",
        "%{wks.location}/Hydrogen/3rdParty/glm",
        "%{wks.location}/Hydrogen/"
    }

    libdirs{
        "%{wks.location}/Hydrogen/Engine/Glew",
		"%{wks.location}/bin/"..OutputDir.."/glfw",
		"%{wks.location}/bin/"..OutputDir.."/imgui",
		"%{wks.location}/bin/"..OutputDir.."/stb_image",
		"%{wks.location}/bin/"..OutputDir.."/SimdJson",
        "%{wks.location}/bin/"..OutputDir.."/Xenon"
    }
    links {"opengl32", "glew32s", "glfw", "imgui", "stb_image", "SimdJson", "Xenon"}

	postbuildcommands {"{COPY} %{wks.location}bin/"..OutputDir.."/Hydrogen/Hydrogen.dll   %{wks.location}bin/"..OutputDir.."/SandBox"}
