include(FetchContent)

add_subdirectory( "glfw-3.3.4" )
add_subdirectory( "nlohmann" )

#FetchContent_MakeAvailable(libpng)
#add_library(PNG::PNG ALIAS png_static)

#FetchContent_Declare(
#	msdf-atlas-gen
#	GIT_REPOSITORY https://github.com/Chlumsky/msdf-atlas-gen.git
#	#GIT_TAG v1.2.2
#)

#set(MSDF_ATLAS_MSDFGEN_EXTERNAL ON)
#FetchContent_MakeAvailable(msdf-atlas-gen)


#set(MSDFGEN_CORE_ONLY OFF)
#set(MSDFGEN_BUILD_STANDALONE OFF)
#set(MSDFGEN_USE_CPP11 OFF)
#set(MSDFGEN_INSTALL OFF)
#set(MSDFGEN_USE_VCPKG ON)
#add_subdirectory( "msdfgen" )

set(ASSIMP_BUILD_ZLIB FALSE)
add_subdirectory( "assimp-5.3.1" )
