cmake_minimum_required (VERSION 3.8)

set(SRC_FILES
	"src/GameRuntime.cpp"
 )

set( HEADER_FILES 
	"inc/GameRuntime.h"
)

add_executable ( GameRuntime ${SRC_FILES} ${HEADER_FILES} )

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/inc" PREFIX "Include" FILES ${HEADER_FILES})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" PREFIX "Source" FILES ${SRC_FILES})

target_include_directories( GameRuntime 
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc/ 
)

add_dependencies( GameRuntime GameProject )

target_link_libraries( GameRuntime
	GameProject
)