workspace "Hydrogen"
  configurations { "Debug", "Release",  "Distrib" }
  platforms {"x64"}
  OutputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
  architecture "x86_64"

  filter {"configurations:Distrib"}
    defines {"DIST"}
    optimize "on"
    symbols "off"
    buildoptions {"/MD"}
    runtime "Release"
  filter {"system:windows"}
    defines {"WINDOWS"}
    filter {"configurations:Debug"}
      defines {"DEBUG"}
      buildoptions {"/MDd"}
    filter {"configurations:Release"}
      defines {"RELEASE"}
      optimize "on"
      symbols "off"
      buildoptions {"/MD"}
      runtime "Release"
  

  flags
  {
  		"MultiProcessorCompile"
  }
  
  startproject "SandBox"
    

  group "Dependencies"
    include "./Hydrogen/3rdParty/imgui"
    include "./Hydrogen/3rdParty/stb_image"
    include "./Hydrogen/3rdParty/glfw"
    include "./Hydrogen/3rdParty/SimdJson"
  group ""

  group "Core"
    include "./Hydrogen"
    include "./Hydrogen/Xenon"
  group ""

  group "Misc"
    include "./SandBox"
  group ""


