project "Xenon"
    location "./"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    targetdir ("%{wks.location}/bin/"..OutputDir.."/Xenon")
    objdir ("%{wks.location}/bin-obj/"..OutputDir.."/Xenon")
    files
    {
        "./**.h",
        "./**.cpp"
    }
    includedirs 
    {
        "./include/",
        "../3rdParty/"
    }

    libdirs{
     "../3rdParty/SimdJson/"
    }
    links{"SimdJson"}

    buildoptions{
        "-fPIC"
    }