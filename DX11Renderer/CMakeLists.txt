cmake_minimum_required ( VERSION 3.12.1 )

set( SRC_FILES
	"src/DX11Renderer.cpp" "src/DX11Pipeline.cpp" "src/DX11Window.cpp" "src/DX11Shader.cpp" "src/DX11Material.cpp" "src/DX11Mesh.cpp" "src/DX11VertexBuffer.cpp" "src/Texture/DX11Texture2D.cpp" "src/ShaderIncluder.cpp")

set( HEADER_FILES 
	"inc/DX11Renderer.h" "inc/DX11Pipeline.h" "inc/DX11Window.h" "inc/DX11Shader.h" "inc/DX11Material.h" "inc/DX11Mesh.h" "inc/DX11VertexBuffer.h" "inc/Texture/DX11Texture2D.h" "inc/ShaderIncluder.h")

add_library ( DX11Renderer ${SRC_FILES} ${HEADER_FILES} )

SF_SETUP_MODULE(DX11Renderer)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/inc" PREFIX "Include" FILES ${HEADER_FILES})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" PREFIX "Source" FILES ${SRC_FILES})

add_dependencies( DX11Renderer Rendering )

target_link_libraries( DX11Renderer 
	PUBLIC Rendering
	PRIVATE d3d11
	PRIVATE d3dcompiler
	PRIVATE dxgi
	PRIVATE dxguid
)