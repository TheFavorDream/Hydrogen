workspace "Hydrogen"
  configurations { "Debug", "Release",  "Distrib" }
  platforms {"x64"}
  OutputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
  architecture "x86_64"

  filter {"configurations:Distrib"}
  defines {"DIST"}
  optimize "on"
  
  filter {"system:windows"}
    defines {"WINDOWS"}
    filter {"configurations:Debug"}
      defines {"DEBUG"}
      buildoptions {"/MTd"}
    filter {"configurations:Release"}
      defines {"RELEASE"}
      optimize "on"
      symbols "off"
      buildoptions {"/MT"}
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
  group ""

  group "Core"
    include "./Hydrogen"
  group ""

  group "Misc"
    include "./SandBox"
  group ""


