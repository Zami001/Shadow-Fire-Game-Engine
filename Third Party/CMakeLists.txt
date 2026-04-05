include(FetchContent)

add_subdirectory( "glfw-3.3.4" )
add_subdirectory( "nlohmann" )

#FetchContent_MakeAvailable(libpng)
#add_library(PNG::PNG ALIAS png_static)

set(MSDFGEN_CORE_ONLY OFF)
set(MSDFGEN_BUILD_STANDALONE OFF)
set(MSDFGEN_USE_CPP11 OFF)
set(MSDFGEN_INSTALL OFF)
set(MSDFGEN_USE_VCPKG ON)

FetchContent_Declare(
	msdf-atlas-gen
	GIT_REPOSITORY https://github.com/Chlumsky/msdf-atlas-gen.git
	GIT_TAG v1.3
	GIT_SUBMODULES "msdfgen;artery-font-format"
	GIT_SUBMODULES_RECURSE TRUE
)

#set(MSDF_ATLAS_MSDFGEN_EXTERNAL ON)
FetchContent_MakeAvailable(msdf-atlas-gen)

#ExternalProject_Add(
#	msdf-atlas-gen
#	GIT_REPOSITORY https://github.com/Chlumsky/msdf-atlas-gen.git
#	GIT_TAG v1.3
#	GIT_SUBMODULES ""
#	GIT_SUBMODULES_RECURSE TRUE
#)

set(MSDF_ATLAS_USE_SKIA OFF)
set(MSDFGEN_USE_SKIA OFF)

#add_subdirectory( "msdf-atlas-gen" )

set(ASSIMP_BUILD_ZLIB FALSE)
add_subdirectory( "assimp-5.3.1" )

add_subdirectory( "miniaudio" )
