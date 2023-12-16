#include <Shader.h>

bool Shader::BuildShader(const char* str) {
	if (isValid) {
		SF_LOG(LogShader, Error, "Shader has already been compiled, must use a new shader to compile again");
		return false;
	}

	isValid = CompileShader(str);
	return isValid;
}