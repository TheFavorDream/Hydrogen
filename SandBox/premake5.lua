project "SandBox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"

    targetdir ("%{wks.location}/bin/"..OutputDir.."/SandBox")
    objdir ("%{wks.location}/bin-obj/"..OutputDir.."/SandBox")
    
    files {
     "./**.cpp",
     "./**.h"
    }

    includedirs{
     "%{wks.location}/SandBox/",
     "%{wks.location}/Hydrogen/",
     "%{wks.location}/Hydrogen/Engine/",
     "%{wks.location}/Hydrogen/3rdParty/glfw/include",
     "%{wks.location}/Hydrogen/3rdParty/imgui",
     "%{wks.location}/Hydrogen/3rdParty",
     "%{wks.location}/Hydrogen/3rdParty/Vulkan"
     }
     
    libdirs{
     "%{wks.location}/bin/"..OutputDir.."/Hydrogen"
    }
     
    links {"Hydrogen"}
	
    --postbuildcommands {"{COPY} %{wks.location}/bin/" .. OutputDir .. "/Hydrogen/Hydrogen.dll %{wks.location}/bin/" .. OutputDir .. "/SandBox",
    --                "{COPY} %{wks.location}/bin/" .. OutputDir .. "/glfw/glfw.dll %{wks.location}bin/" .. OutputDir .. "/SandBox"}


 
