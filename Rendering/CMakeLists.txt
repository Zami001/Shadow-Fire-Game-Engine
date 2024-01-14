cmake_minimum_required ( VERSION 3.12.1 )

set(SRC_FILES
	"src/Rendering.cpp"
  "src/RenderPipeline.cpp" "src/Window.cpp" "src/Shader.cpp" "src/VertexBuffer.cpp" "src/RenderTarget.cpp" "src/Camera.cpp" "src/MeshImporter.cpp" "src/Skeleton.cpp" "src/Texture/Texture2D.cpp" "src/SkeletalAnimation.cpp" "src/Mesh.cpp" "src/UI/UIElement.cpp" "src/UI/Panels/UIPanel.cpp" "src/UI/Panels/OverlayPanel.cpp" "src/ShaderImporter.cpp" "src/Material.cpp" "src/UI/Panels/UIRoot.cpp" "src/UI/UIImage.cpp" "src/WindowInput.cpp")

set( HEADER_FILES 
	"inc/Rendering.h"
 "inc/RenderPipeline.h" "inc/Window.h" "inc/Camera.h" "inc/Shader.h" "inc/Material.h" "inc/Mesh.h" "inc/VertexBuffer.h"  "inc/RenderTarget.h" "inc/MeshImporter.h" "inc/Skeleton.h" "inc/Texture/Texture2D.h"   "inc/SkeletalAnimation.h" "inc/UI/UIElement.h" "inc/UI/Panels/UIPanel.h" "inc/UI/Panels/OverlayPanel.h" "inc/ShaderImporter.h" "inc/UI/Panels/UIRoot.h" "inc/UI/UIImage.h" "inc/WindowInput.h" "inc/UI/UIEventResponse.h")

add_library ( Rendering ${HEADER_FILES} ${SRC_FILES} )

SF_SETUP_MODULE(Rendering)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/inc" PREFIX "Include" FILES ${HEADER_FILES})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" PREFIX "Source" FILES ${SRC_FILES})

add_dependencies( Rendering Core )
add_dependencies( Rendering assimp )

target_link_libraries( Rendering
	PUBLIC Core
	PRIVATE assimp
)