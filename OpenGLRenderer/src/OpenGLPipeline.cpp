#include <OpenGLPipeline.h>
#include <OpenGLWindow.h>
#include <stdlib.h>
#include <stdio.h>
#include <OpenGLMesh.h>
#include <OpenGLMaterial.h>
#include <OpenGLShader.h>
#include <OpenGLRenderTarget.h>

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
//#include <GLFW/glfw3.h>

static const char* vertex_shader_text =
"#version 110\n"
"uniform mat4 MVP;\n"
"attribute vec3 vCol;\n"
"attribute vec2 vPos;\n"
"varying vec3 color;\n"
"void main()\n"
"{\n"
"    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
"    color = vCol;\n"
"}\n";

static const char* fragment_shader_text =
"#version 110\n"
"varying vec3 color;\n"
"void main()\n"
"{\n"
"    gl_FragColor = vec4(color, 1.0);\n"
"}\n";

OpenGLPipeline* OpenGLPipeline::CreatePipeline() {
	return new OpenGLPipeline();
}

void OpenGLPipeline::Init() {
	glfwSetErrorCallback(Error_Callback);

	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}
}

void OpenGLPipeline::Shutdown() {
	glfwTerminate();
}

Window* OpenGLPipeline::CreateWindow(const WindowParams& params) {
	return new OpenGLWindow(params);
}

Mesh* OpenGLPipeline::CreateMesh() {
	return new OpenGLMesh();
}

Material* OpenGLPipeline::GetDefaultMaterial() const {
	OpenGLShader* vert = new OpenGLShader(ShaderType::Vertex);
	vert->CompileShader(vertex_shader_text, ShaderType::Vertex);

	OpenGLShader* frag = new OpenGLShader(ShaderType::Fragment);
	frag->CompileShader(fragment_shader_text, ShaderType::Fragment);

	Shader* shaders[2] = {vert, frag};

	OpenGLMaterial* material = new OpenGLMaterial();
	material->SetupMaterial(shaders, 2);

	return material;
}

RenderTarget* OpenGLPipeline::CreateRenderTarget(const Bounds2DInt& bounds) {
	return new OpenGLRenderTarget(bounds);
}

void OpenGLPipeline::BeginRender(const Camera& camera) {
	
}

void OpenGLPipeline::EndRender(const Camera& camera) {
	
}

void OpenGLPipeline::Error_Callback(int error, const char* description) {
	fprintf(stderr, "Error: %s\n", description);
}