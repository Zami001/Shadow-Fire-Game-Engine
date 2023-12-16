#pragma once

#include <OpenGLRenderer.h>
#include <Material.h>

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

class OPENGL_API OpenGLMaterial : public Material {
	friend class OpenGLMesh;

public:
	OpenGLMaterial();
	~OpenGLMaterial();

	virtual void SetupMaterial(Shader** shaders, int count) override;

private:
	GLuint handle;
};