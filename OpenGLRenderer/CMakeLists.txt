cmake_minimum_required ( VERSION 3.12.1 )

set(GLAD_GL "${GLFW_SOURCE_DIR}/deps/glad/gl.h"
            "${GLFW_SOURCE_DIR}/deps/glad_gl.c")

set( SRC_FILES
	"src/OpenGLRenderer.cpp"
  "src/OpenGLPipeline.cpp" "src/OpenGLWindow.cpp" "src/OpenGLMesh.cpp" "src/OpenGLMaterial.cpp" "src/OpenGLShader.cpp" "src/OpenGLRenderTarget.cpp" "src/OpenGLWindowRenderTarget.cpp" "src/OpenGLInputManager.cpp")

set( HEADER_FILES 
	"inc/OpenGLRenderer.h"
	"inc/OpenGLPipeline.h"
 "inc/OpenGLWindow.h" "inc/OpenGLMesh.h" "inc/OpenGLMaterial.h" "inc/OpenGLShader.h" "inc/OpenGLRenderTarget.h" "inc/OpenGLWindowRenderTarget.h" "inc/OpenGLInputManager.h")

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