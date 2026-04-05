cmake_minimum_required ( VERSION 3.12.1 )

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS "src/*")
file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS "inc/*")

add_library ( Audio ${HEADER_FILES} ${SRC_FILES} )

SF_SETUP_MODULE(Audio)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/inc" PREFIX "Include" FILES ${HEADER_FILES})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" PREFIX "Source" FILES ${SRC_FILES})

add_dependencies( Audio Core )

target_link_libraries( Audio
	PUBLIC Core
	PRIVATE miniaudio
)

target_link_libraries( Audio
	PUBLIC miniaudio
)