cmake_minimum_required (VERSION 3.8)

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS "src/*")
file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS "inc/*")

# Add source to this project's executable.
add_library ( Engine ${HEADER_FILES} ${SRC_FILES} )

SF_SETUP_MODULE(Engine)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/inc" PREFIX "Include" FILES ${HEADER_FILES})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" PREFIX "Source" FILES ${SRC_FILES})

add_dependencies( Engine Rendering )
add_dependencies( Engine Core )
add_dependencies( Engine Audio )

target_link_libraries ( Engine
	Rendering
	Core
	Audio
)

add_subdirectory( tests )