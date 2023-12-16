#include <OpenGLMaterial.h>
#include <OpenGLShader.h>
#include <Asserts.h>

OpenGLMaterial::OpenGLMaterial() {
	handle = glCreateProgram();
}

OpenGLMaterial::~OpenGLMaterial() {
	glDeleteProgram(handle);
}

void OpenGLMaterial::SetupMaterial(Shader** shaders, int count) {
	for (int i = 0; i < count; ++i) {
		SF_ASSERT(dynamic_cast<OpenGLShader*>(shaders[i]) != nullptr, "Shader passed to setup material was nullptr or not a valid OpenGLShader")
		OpenGLShader* shader = reinterpret_cast<OpenGLShader*>(shaders[i]);
		glAttachShader(handle, shader->handle);
	}

	glLinkProgram(handle);
}
