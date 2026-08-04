project "ImGui"
    kind "StaticLib"
	language "C++"
    cppdialect "C++17"
    location "./."

    targetdir ("%{wks.location}/bin/"..OutputDir.."/Imgui")
    objdir ("%{wks.location}/bin-obj/"..OutputDir.."/imgui")


    files {
        "**.cpp",
        "**.h"
    }

    includedirs{
        "%{wks.location}/Hydrogen/3rdParty/glfw/include/",
        "%{wks.location}/Hydrogen/Source/"
    }
    

    filter "system:linux"
        pic "On"
    