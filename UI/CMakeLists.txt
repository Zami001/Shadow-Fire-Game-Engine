cmake_minimum_required ( VERSION 3.12.1 )

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS "src/*")
file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS "inc/*")

add_library ( UI ${HEADER_FILES} ${SRC_FILES} )

SF_SETUP_MODULE(UI)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/inc" PREFIX "Include" FILES ${HEADER_FILES})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" PREFIX "Source" FILES ${SRC_FILES})

add_dependencies( UI Core )
add_dependencies( UI Rendering )
add_dependencies( UI msdf-atlas-gen )

target_link_libraries( UI
	PUBLIC Core
	PUBLIC Rendering
)

target_link_libraries( UI
	PUBLIC msdf-atlas-gen
)