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
     "%{wks.location}/Hydrogen/Source/",
     "%{wks.location}/Hydrogen/3rdParty/glfw/include",
     "%{wks.location}/Hydrogen/3rdParty/imgui",
     "%{wks.location}/Hydrogen/3rdParty"
     }
     
    libdirs{
     "%{wks.location}/bin/"..OutputDir.."/Hydrogen"
    }
     
    links {"Hydrogen","kernel32", "user32" , "gdi32", "winspool", "comdlg32", "advapi32", "shell32","ole32", "oleaut32", "uuid", "odbc32", "odbccp32"}
	
    postbuildcommands {"{COPY} %{wks.location}/bin/" .. OutputDir .. "/Hydrogen/Hydrogen.dll %{wks.location}/bin/" .. OutputDir .. "/SandBox",
                    "{COPY} %{wks.location}/bin/" .. OutputDir .. "/glfw/glfw.dll %{wks.location}bin/" .. OutputDir .. "/SandBox"}


 
