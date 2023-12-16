#include <OpenGLShader.h>

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

void OpenGLShader::CompileShader(const char* str, ShaderType shaderType) {
	switch (shaderType) {
		case ShaderType::Vertex:
			handle = glCreateShader(GL_VERTEX_SHADER);
			break;

		case ShaderType::Fragment:
			handle = glCreateShader(GL_FRAGMENT_SHADER);
			break;
	}

	glShaderSource(handle, 1, &str, NULL);
	glCompileShader(handle);
}