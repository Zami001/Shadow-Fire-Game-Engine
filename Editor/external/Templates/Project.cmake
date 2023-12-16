cmake_minimum_required(VERSION 3.18)

set(SRC_FILES 
	##SRC_FILES##
)

set( HEADER_FILES 
	##HEADER_FILES##
)

set( SF_PROJECT_NAME 
	##PROJECT_NAME##
)

project ( "${SF_PROJECT_NAME}" LANGUAGES CXX VERSION 0.1 )

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set_property( GLOBAL PROPERTY USE_FOLDERS ON )

if(NOT DEFINED SF_ENGINE_BUILD)
	find_package ( ShadowFire REQUIRED
		PATHS "C:/Shadow Fire Game Engine/out/install/x64-Debug/lib/cmake/ShadowFire"
	)

	set(SF_EDITOR True)

	option( SF_USE_ASSERTS "Use asserts while running shadow fire functions" True )

	add_definitions(
		-DSF_USE_ASSERTS=$<BOOL:${SF_USE_ASSERTS}>
		-DSF_EDITOR=$<BOOL:${SF_EDITOR}>
	)
endif()

string( REGEX REPLACE " " "" SF_TARGET_NAME ${SF_PROJECT_NAME} )

if(${SF_EDITOR})
	add_library ( ${SF_TARGET_NAME} SHARED ${SRC_FILES} ${HEADER_FILES} )
	set_target_properties( ${SF_TARGET_NAME} PROPERTIES OUTPUT_NAME "${SF_PROJECT_NAME}-Editor" )
	set_target_properties( ${SF_TARGET_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_LIST_DIR}/../Binaries" )
	set_target_properties( ${SF_TARGET_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_LIST_DIR}/../Binaries" )
	set_target_properties( ${SF_TARGET_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_LIST_DIR}/../Binaries" )
else()
	add_library ( ${SF_TARGET_NAME} ${SRC_FILES} ${HEADER_FILES} )
endif()

add_library ( GameProject ALIAS ${SF_TARGET_NAME} )

target_include_directories( ${SF_TARGET_NAME}
	PUBLIC "${CMAKE_CURRENT_LIST_DIR}/../Scripts/inc"
)

target_link_libraries ( ${SF_TARGET_NAME} 
	PUBLIC OpenGLRenderer
	PUBLIC Engine
)

source_group(TREE "${CMAKE_CURRENT_LIST_DIR}/../Scripts/inc" PREFIX "Include" FILES ${HEADER_FILES})
source_group(TREE "${CMAKE_CURRENT_LIST_DIR}/../Scripts/src" PREFIX "Source" FILES ${SRC_FILES})

set_directory_properties( PROPERTIES VS_STARTUP_PROJECT ${SF_TARGET_NAME} )