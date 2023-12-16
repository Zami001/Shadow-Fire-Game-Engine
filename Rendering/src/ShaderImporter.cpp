#include <ShaderImporter.h>

ShaderImporter::ShaderImporter() {
	InsertZeroChar = true;
}

bool ShaderImporter::ConvertData(Shader& Output, const char* data, size_t dataLength) {
	return Output.BuildShader(data);
}