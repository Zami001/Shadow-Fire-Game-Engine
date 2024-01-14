cmake_minimum_required (VERSION 3.8)

set(SRC_FILES
	"src/Engine.cpp"
 "src/Application.cpp" "src/Game.cpp" "src/Scene.cpp" "src/Components/Component.cpp" "src/Components/SceneComponent.cpp" "src/Components/StaticMeshComponent.cpp" "src/GameObject.cpp" "src/TickManager.cpp" "src/Assets/SerializedAsset.cpp" "src/SFGUID.cpp" "src/SFObject.cpp" "src/Assets/AssetCache.cpp" "src/Assets/AssetFactory.cpp" "src/Logging/ConsoleLogger.cpp" "src/Components/SkeletalMeshComponent.cpp" "src/Components/MeshComponent.cpp" "src/Components/UIComponent.cpp")

set( HEADER_FILES 
	"inc/Engine.h"
 "inc/Application.h" "inc/Game.h"  "inc/Scene.h" "inc/GameObject.h" "inc/Components/SceneComponent.h" "inc/Components/Component.h" "inc/Components/StaticMeshComponent.h"  "inc/TickManager.h" "inc/Assets/AssetType.h" "inc/Assets/AssetFactory.h" "inc/SFObject.h"  "inc/StreamBuffers/StreamBuffer.h" "inc/StreamBuffers/FileBuffer.h"  "inc/Assets/SerializedAsset.h" "inc/SFGUID.h" "inc/Assets/AssetCache.h"  "inc/Logging/ConsoleLogger.h" "inc/Components/SkeletalMeshComponent.h" "inc/Components/MeshComponent.h"  "inc/Components/UIComponent.h" )

# Add source to this project's executable.
add_library ( Engine ${HEADER_FILES} ${SRC_FILES} )

SF_SETUP_MODULE(Engine)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/inc" PREFIX "Include" FILES ${HEADER_FILES})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" PREFIX "Source" FILES ${SRC_FILES})

add_dependencies( Engine Rendering )
add_dependencies( Engine Core )

target_link_libraries ( Engine
	Rendering
	Core
)