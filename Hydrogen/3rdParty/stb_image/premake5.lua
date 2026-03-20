project "Stb_image"
    language "C++"
    kind "StaticLib"

    targetdir ("%{wks.location}/bin/"..OutputDir.."/stb_image")
    objdir ("%{wks.location}/bin-obj/"..OutputDir.."/stb_image")
        
    files{
        "stb_image.h",
        "stb_image.cpp"
    }

    