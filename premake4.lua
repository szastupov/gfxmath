project "gfxmath"
        kind "StaticLib"
        language "C++"
        files { "**.h", "**.cpp" }
        excludes { "math_test.cpp" }