cmake_minimum_required ( VERSION 3.12.1 )

set(GLAD_GL "${GLFW_SOURCE_DIR}/deps/glad/gl.h"
            "${GLFW_SOURCE_DIR}/deps/glad_gl.c")

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS "src/*")
file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS "inc/*")

add_library ( OpenGLRenderer ${SRC_FILES} ${HEADER_FILES} ${GLAD_GL} )

SF_SETUP_MODULE(OpenGLRenderer)

target_include_directories( OpenGLRenderer 
	PRIVATE "${GLFW_SOURCE_DIR}/deps"
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/inc" PREFIX "Include" FILES ${HEADER_FILES})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" PREFIX "Source" FILES ${SRC_FILES})

add_dependencies( OpenGLRenderer glfw )
add_dependencies( OpenGLRenderer Rendering )

target_link_libraries( OpenGLRenderer 
	PRIVATE glfw
	PUBLIC Rendering
)