cmake_minimum_required (VERSION 3.24)

set(SF_ENGINE_BUILD ON)
set(SF_PROJECT_PATH "" CACHE PATH "Path to shadow fire project game to package")

if(SF_PROJECT_PATH STREQUAL "")
    project( "Shadow Fire Engine" LANGUAGES CXX C VERSION 0.1 )
else()
    include( "${SF_PROJECT_PATH}/Intermediate/CMakeLists.txt" )
endif()

enable_testing()

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

    #install ( TARGETS ${tar} 
	#    EXPORT ${tar}Targets
	#    RUNTIME
    #)

    #install ( EXPORT ${tar}Targets 
	#    FILE ${tar}Targets.cmake
	#    DESTINATION lib/cmake/${tar}
    #)

    #install ( DIRECTORY inc/ DESTINATION include/${tar} FILE_PERMISSIONS WORLD_READ )
    #install ( DIRECTORY src/ DESTINATION source/${tar} FILE_PERMISSIONS WORLD_READ )

    #write_basic_package_version_file (
    #    "${CMAKE_CURRENT_BINARY_DIR}/${tar}ConfigVersion.cmake"
    #    VERSION "${${tar}_VERSION_MAJOR}.${${tar}_VERSION_MINOR}"
    #    COMPATIBILITY AnyNewerVersion
    #)

    #install ( FILES
    #    ${CMAKE_CURRENT_BINARY_DIR}/${tar}ConfigVersion.cmake
    #    DESTINATION lib/cmake/${tar}
    #)

    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/external")
        add_custom_command(
	        TARGET ${tar} POST_BUILD
	        COMMAND "${CMAKE_COMMAND}" -E copy_directory
			"${CMAKE_CURRENT_SOURCE_DIR}/external/"
			"$<TARGET_FILE_DIR:${tar}>/"
        )
    endif()
endmacro()

macro(SF_SETUP_TESTS tar)
    file(GLOB_RECURSE TEST_FILES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" CONFIGURE_DEPENDS "*.cpp")
    create_test_sourcelist(TestSources ${tar}Test.cpp ${TEST_FILES})

    add_executable(${tar}Test ${TestSources})

    target_link_libraries(${tar}Test PRIVATE ${tar})

    foreach(test ${TEST_FILES})
	    get_filename_component(TName ${test} NAME_WE)
	    get_filename_component(TDirectory ${test} DIRECTORY)
	
	    if(TDirectory STREQUAL "")
		    set(TDirectoryPrefix "")
		    set(TDirectory "General")
	    else()
		    set(TDirectoryPrefix "${TDirectory}/")
	    endif()

	    add_test(NAME "${tar}.${TDirectory}.${TName}" COMMAND ${tar}Test "${TDirectoryPrefix}${TName}")
    endforeach()
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
add_subdirectory ( DX11Renderer )

#OpenGL has been temporarily abanadoned and as a result is heavily broken. not adding the subdirectory so that tests will continue to build properly
#add_subdirectory ( OpenGLRenderer )

# Set the startup project.
if(SF_PROJECT_PATH STREQUAL "")
    set_directory_properties( PROPERTIES VS_STARTUP_PROJECT Engine )
else()
	set_directory_properties( PROPERTIES VS_STARTUP_PROJECT ${SF_TARGET_NAME} )
endif()

#configure_package_config_file ( ${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
#    "${CMAKE_CURRENT_BINARY_DIR}/ShadowFireConfig.cmake"
#    INSTALL_DESTINATION "lib/cmake/ShadowFire"
#    NO_SET_AND_CHECK_MACRO
#    NO_CHECK_REQUIRED_COMPONENTS_MACRO
#)

#install ( FILES
#    ${CMAKE_CURRENT_BINARY_DIR}/ShadowFireConfig.cmake
#    DESTINATION lib/cmake/ShadowFire
#)