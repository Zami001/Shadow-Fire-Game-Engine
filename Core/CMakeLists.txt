cmake_minimum_required (VERSION 3.8)

set(SRC_FILES
	"src/Core.cpp"
   "src/Math/Matrix4x4.cpp" "src/Profiling/ScopedProfiler.cpp" "src/Profiling/TimedProfiler.cpp"  "src/Math/Random.cpp"   "src/Input/InputManager.cpp"  "src/Logging/Log.cpp" "src/Logging/LogManager.cpp" "src/Logging/LogOutput.cpp" "src/Math/Transform.cpp" "src/Math/Quaternion.cpp"  "src/Math/Vector.cpp")

set( HEADER_FILES 
	"inc/Core.h"
 "inc/Math/Matrix4x4.h"   "inc/Math/Bounds.h" "inc/Profiling/ScopedProfiler.h"  "inc/Profiling/TimedProfiler.h" "inc/Input/Keycodes.h" "inc/Input/InputManager.h" "inc/Math/Random.h" "inc/Asserts.h" "inc/Platform/Windows/PostWindowsInclude.h" "inc/Platform/Windows/PreWindowsInclude.h" "inc/FileImporter.h" "inc/Math/Constants.h" "inc/Logging/LogOutput.h" "inc/Logging/Log.h" "inc/Logging/LogManager.h" "inc/Math/Transform.h"  "inc/Math/Quaternion.h"  "inc/Math/Vector.h"   "inc/Containers/SFObjectContainer.h" "inc/MinimalCore.h" )

add_library ( Core ${SRC_FILES} ${HEADER_FILES} )

SF_SETUP_MODULE(Core)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/inc" PREFIX "Include" FILES ${HEADER_FILES})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" PREFIX "Source" FILES ${SRC_FILES})