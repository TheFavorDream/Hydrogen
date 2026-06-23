project "SimdJson"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    location "./."

    files
    {
        "**.h",
        "**.cpp"
    }

    targetdir ("%{wks.location}/bin/"..OutputDir.."/SimdJson")
    objdir ("%{wks.location}/bin-obj/"..OutputDir.."/SimdJson")