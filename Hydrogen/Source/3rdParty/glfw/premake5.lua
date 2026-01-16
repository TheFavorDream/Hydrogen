workspace "GLFW"
    architecture "x64"
    configurations {"Debug", "Release"}

    project "GLFW"
        kind "StaticLib"
        language "C"

        targetdir ("./bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/GLFW")
        objdir ("./bin-in/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/GLFW")

        files {
            "./include/**.h",
            "./src/**.c"
        }

        filter ("system:windows")
            systemversion "10.0.17763.0"
            staticruntime "On"

            defines {
                "_WIN32", "_GLFW_WIN32"
            }


        filter ("configurations:Debug")
            symbols "On"
        filter ("configurations:Release")
            optimize "On"

        filter {"system:windows", "configurations:Debug"}
            buildoptions {"/MTd"}
