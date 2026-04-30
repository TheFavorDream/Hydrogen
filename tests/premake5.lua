workspace "tests"
    configurations {"Debug", "Release"}
    platforms {"x64"}
    filter {"configurations:Debug"}
        defines {"DEBUG"}
        optimize "off"

    filter {"configurations:Release"}
        defines {"RELEASE"}
        optimize "on"




    project "LinkedList_test"
            location "./Misc_tests/"
            kind "ConsoleApp"
            language "C++"

            files {
                "./Misc_tests/**.cpp",
                "./Misc_tests/**.h"
            }

            includedirs {
                "../Hydrogen/Source/"
            }

    project "VecMath_test"
            location "./VecMath_tests/"
            kind "ConsoleApp"
            language "C++"



            files {
                "./VecMath_tests/**.cpp",
                "./VecMath_tests/**.h",
                "../Hydrogen/Source/VecMath/**.cpp",
                "../Hydrogen/Source/VecMath/**.h"
            }

            includedirs {
                "../Hydrogen/Source/",
                "../Tools/Hydrofiler/Source/"
            }

            libdirs{
                "../Tools/Hydrofiler/bin/Release/"
            }

            links{"Hydrofiler"}

    project "ResourcePool"
            location "./ResourcePool_tests"
            kind "ConsoleApp"
            language "C++"

            files{
                "./ResourcePool_tests/**.cpp",
                "./ResourcePool_tests/**.h",
                "../Hydrogen/Source/Core/ResourcePool.h"
            }
            includedirs {
                "../Hydrogen/Source/"
            }


    project "Memory_Test"
            location "./Memory_tests"
            kind "ConsoleApp"
            language "C++"
            
            files{
                "./Memory_tests/**.cpp",
                "../Hydrogen/Source/Core/Memory.**",
                "../Hydrogen/Source/Core/Memory.**",
            }
            defines{"TEST"}
            includedirs{
                "../Hydrogen/Source/"
            }