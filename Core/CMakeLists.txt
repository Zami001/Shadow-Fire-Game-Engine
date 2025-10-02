cmake_minimum_required (VERSION 3.8)

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS "src/*")
file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS "inc/*")

add_library ( Core ${SRC_FILES} ${HEADER_FILES} )

SF_SETUP_MODULE(Core)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/inc" PREFIX "Include" FILES ${HEADER_FILES})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" PREFIX "Source" FILES ${SRC_FILES})

target_link_libraries( Core
	xinput
)

add_subdirectory(tests)