#pragma once

#include <OpenGLRenderer.h>
#include <Shader.h>

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

class OPENGL_API OpenGLShader : public Shader {
	friend class OpenGLMaterial;
	friend class OpenGLPipeline;

public:
	OpenGLShader(ShaderType type) : Shader(type), handle(0) {}

	virtual void CompileShader(const char* str, ShaderType shaderType) override;

private:
	GLuint handle;
};