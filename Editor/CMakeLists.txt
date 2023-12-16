cmake_minimum_required (VERSION 3.18)

set(SRC_FILES
	"src/Editor.cpp"
  "src/EditorGame.cpp" "src/Project.cpp")

set( HEADER_FILES 
	"inc/Editor.h"
 "inc/EditorGame.h" "inc/Project.h")

add_executable ( Editor ${SRC_FILES} ${HEADER_FILES} "resources/resources.rc" )
#add_executable ( Editor WIN32 ${SRC_FILES} ${HEADER_FILES} )

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/inc" PREFIX "Include" FILES ${HEADER_FILES})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" PREFIX "Source" FILES ${SRC_FILES})

set_target_properties( Editor PROPERTIES OUTPUT_NAME "Shadow Fire Editor" )
set_target_properties( Editor PROPERTIES FOLDER "Shadow Fire")

add_custom_command(
	TARGET Editor POST_BUILD
	COMMAND "${CMAKE_COMMAND}" -E copy_directory
			"${CMAKE_CURRENT_SOURCE_DIR}/external/"
			"$<TARGET_FILE_DIR:Editor>/"
)

target_include_directories( Editor 
	PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/inc/" 
)

add_dependencies ( Editor Engine )
add_dependencies ( Editor DX11Renderer )
#add_dependencies ( Editor OpenGLRenderer )

target_link_libraries ( Editor
	Engine
	DX11Renderer
)

#if (MSVC)
#    # Tell MSVC to use main instead of WinMain for Windows subsystem executables
#    set_target_properties( Editor PROPERTIES
#                          LINK_FLAGS "/ENTRY:mainCRTStartup")
#endif()