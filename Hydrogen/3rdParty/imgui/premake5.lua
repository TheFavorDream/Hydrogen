project "ImGui"
    kind "StaticLib"
	language "C++"

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
    
