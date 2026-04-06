project "Hydrofiler"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"

    targetdir ("%{wks.location}/Hydrofiler/bin/"..OutputDir.."/")
    objdir ("%{wks.location}/Hydrofiler/bin-obj/"..OutputDir.."/")

    files{
        "Source/**.h",
        "Source/**.cpp"
    }

    includedirs {
        "Dependent/"
    }

    libdirs{
        "Dependent/"
    }
