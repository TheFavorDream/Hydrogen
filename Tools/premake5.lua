workspace "Tools"
    configurations {"Debug", "Release", "Distribute"}
    platforms {"x64"}

    OutputDir = "%{cfg.buildcfg}"
    architecture "x86_64"

    filter {"configurations:Distrib"}
        defines {"DIST"}
        optimize "on"
  
    filter {"system:windows"}
        defines {"WINDOWS"}
        filter {"configurations:Debug"}
            defines {"DEBUG"}
            --buildoptions {"/MTd"}
        filter {"configurations:Release"}
            defines {"RELEASE"}
            optimize "on"
            symbols "off"
            runtime "Release"
  

    flags
    {
  		"MultiProcessorCompile"
    }

include "Hydrofiler"

