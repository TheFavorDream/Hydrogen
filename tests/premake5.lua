workspace "tests"
    configurations {"Debug", "Release"}
    platforms {"x64"}
    filter {"configurations:Debug"}
        defines {"DEBUG"}
        optimize "off"

    filter {"configurations:Release"}
        defines {"RELEASE"}
        optimize "on"

    OutputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"


    project "LinkedList_test"
            location "./Misc_tests/"
            kind "ConsoleApp"
            language "C++"

            files {
                "./Misc_tests/**.cpp",
                "./Misc_tests/**.h"
            }

            includedirs {
                "../Hydrogen/Engine/"
            }

    project "VecMath_tes"
            location "./VecMath_tests/"
            kind "ConsoleApp"
            language "C++"



            files {
                "./VecMath_tests/**.cpp",
                "./VecMath_tests/**.h",
                "../Hydrogen/Engine/VecMath/**.cpp",
                "../Hydrogen/Engine/VecMath/**.h"
            }

            includedirs {
                "../Hydrogen/Engine/",
                "../Tools/Hydrofiler/Engine/"
            }

            libdirs{
                "../Tools/Hydrofiler/bin/Release/"
            }

            links{"Hydrofiler"}

    project "ResourcePool_test"
            location "./ResourcePool_tests"
            kind "ConsoleApp"
            language "C++"

            defines 
            {
                "WINDOWS", "TEST"
            }

            files{
                "./ResourcePool_tests/**.cpp",
                "./ResourcePool_tests/**.h",
                "../Hydrogen/Engine/Core/ResourcePool.h",
                "../Hydrogen/Engine/Core/Memory.h", 
                "../Hydrogen/Engine/Core/Memory.cpp"
            }
            includedirs {
                "../Hydrogen/Engine/"
            }


    project "Memory_test"
            location "./Memory_tests"
            kind "ConsoleApp"
            language "C++"
            
            files{
                "./Memory_tests/**.cpp",
                "../Hydrogen/Engine/Core/Memory.**",
                "../Hydrogen/Engine/Core/Memory.**",
            }
            defines{"TEST"}
            includedirs{
                "../Hydrogen/Engine/"
            }

    include "../Hydrogen/Xenon/"

    project "Xenon_test"
            location "./Xenon_tests"
            kind "ConsoleApp"
            language "C++"
            cppdialect "C++17"

        files 
        {
            "./Xenon_tests/**.cpp"
        }
        includedirs{
            "../Hydrogen/Xenon/",
            "../Tools/Hydrofiler/"
        }
        libdirs{
            "../bin/"..OutputDir.."/Xenon"
        }
        links{
            "Xenon", "Hydrofiler"
        }