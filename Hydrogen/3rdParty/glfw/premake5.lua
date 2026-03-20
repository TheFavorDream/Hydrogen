project "GLFW"
    kind "SharedLib"
    language "C"
    targetdir ("%{wks.location}/bin/"..OutputDir.."/glfw")
    objdir ("%{wks.location}/bin-obj/"..OutputDir.."/glfw")

    files {
        "./include/**.h",
        "./src/**.c"
    }
    filter ("system:windows")
        systemversion "10.0.17763.0"
        staticruntime "On"
        defines {
            "_WIN32", "_GLFW_WIN32", "_GLFW_BUILD_DLL"
        }

