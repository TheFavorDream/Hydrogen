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

        filter {"configurations:Debug"}
            defines {"DEBUG"}
            optimize "off"

        filter {"configurations:Release"}
            defines {"RELEASE"}
            optimize "on"