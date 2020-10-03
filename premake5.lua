workspace "LD47"
  architecture "x86_64"

  configurations { "Debug", "Release" }

project "LD47"
  location "src"
  kind "ConsoleApp"
  language "C++"
  staticruntime "on"
  cppdialect "C++17"

  targetdir ("bin/%{prj.name}" )
  objdir ("obj/%{prj.name}" )

  includedirs {
    "include",
  }

  files {
     "%{prj.location}/**.h",
     "%{prj.location}/**.cpp",
  }

  filter "configurations:Debug"
    defines { "DEBUG" }
    runtime "Debug"
    symbols "on"

  filter "configurations:Release"
    defines { "NDEBUG" }
    runtime "Release"
    optimize "on"

  filter "system:windows"
    defines { "_CRT_SECURE_NO_WARNINGS" }

  filter "system:linux"
    links {
      "GL",
      "X11",
      "png",
      "pthread",
      "dl",
    }
