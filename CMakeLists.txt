cmake_minimum_required (VERSION 3.18)

set(SF_ENGINE_BUILD ON)

if(DEFINED SF_PROJECT_PATH)
    include( "${SF_PROJECT_PATH}/Intermediate/CMakeLists.txt" )
else()
    project( "Shadow Fire Engine" LANGUAGES CXX C VERSION 0.1 )
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/lib) # Output directory for static lib (.LIB)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/bin) # Output directory for shared lib (.DLL)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/bin) # Output directory for executables (.EXE)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set_property( GLOBAL PROPERTY USE_FOLDERS ON )

option( SF_USE_ASSERTS "Use asserts while running shadow fire functions" True )
option( SF_EDITOR "True to compile shadow fire editor" True )
option( BUILD_SHARED_LIBS "True to compile dll modules, false to compile static libraries" True )

add_definitions(
    -DSF_USE_ASSERTS=$<BOOL:${SF_USE_ASSERTS}>
    -DSF_EDITOR=$<BOOL:${SF_EDITOR}>
)

include(CMakePackageConfigHelpers)

set( SF_PACKAGES "" )

macro(SF_SETUP_MODULE tar)
    set_target_properties(${tar} PROPERTIES FOLDER "Shadow Fire")

    set( SF_PACKAGES ${SF_PACKAGES} ${tar} PARENT_SCOPE )

    target_include_directories( ${tar} 
	    PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc/>" "$<INSTALL_INTERFACE:include/${tar}>"
    )

    install ( TARGETS ${tar} 
	    EXPORT ${tar}Targets
	    RUNTIME
    )

    install ( EXPORT ${tar}Targets 
	    FILE ${tar}Targets.cmake
	    DESTINATION lib/cmake/${tar}
    )

    install ( DIRECTORY inc/ DESTINATION include/${tar} FILE_PERMISSIONS WORLD_READ )
    #install ( DIRECTORY src/ DESTINATION source/${tar} FILE_PERMISSIONS WORLD_READ )

    write_basic_package_version_file (
        "${CMAKE_CURRENT_BINARY_DIR}/${tar}ConfigVersion.cmake"
        VERSION "${${tar}_VERSION_MAJOR}.${${tar}_VERSION_MINOR}"
        COMPATIBILITY AnyNewerVersion
    )

    install ( FILES
        ${CMAKE_CURRENT_BINARY_DIR}/${tar}ConfigVersion.cmake
        DESTINATION lib/cmake/${tar}
    )

    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/external")
        add_custom_command(
	        TARGET ${tar} POST_BUILD
	        COMMAND "${CMAKE_COMMAND}" -E copy_directory
			"${CMAKE_CURRENT_SOURCE_DIR}/external/"
			"$<TARGET_FILE_DIR:${tar}>/"
        )
    endif()
endmacro()

if(${SF_EDITOR})
    add_subdirectory ( Editor )
else()
    add_subdirectory ( GameRuntime )
endif()

# Enable multithreaded builds
if( MSVC )
    add_compile_options(/MP)
endif()

#include third party projects
add_subdirectory( "Third Party" )

# Include sub-projects.
add_subdirectory ( Core )
add_subdirectory ( Rendering )
add_subdirectory ( Engine )
add_subdirectory ( OpenGLRenderer )
add_subdirectory ( DX11Renderer )

# Set the startup project.
if(DEFINED SF_PROJECT_PATH)
	set_directory_properties( PROPERTIES VS_STARTUP_PROJECT ${SF_TARGET_NAME} )
else()
    set_directory_properties( PROPERTIES VS_STARTUP_PROJECT Engine )
endif()

configure_package_config_file ( ${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/ShadowFireConfig.cmake"
    INSTALL_DESTINATION "lib/cmake/ShadowFire"
    NO_SET_AND_CHECK_MACRO
    NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

install ( FILES
    ${CMAKE_CURRENT_BINARY_DIR}/ShadowFireConfig.cmake
    DESTINATION lib/cmake/ShadowFire
)