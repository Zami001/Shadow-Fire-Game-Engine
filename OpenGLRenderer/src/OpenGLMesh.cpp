#include <OpenGLMesh.h>

#include <Asserts.h>
#include <OpenGLMaterial.h>

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "linmath.h"

static const struct {
	float x, y;
	float r, g, b;
} vertices[3] =
{
	{ -0.6f, -0.4f, 1.f, 0.f, 0.f },
	{  0.6f, -0.4f, 0.f, 1.f, 0.f },
	{   0.f,  0.6f, 0.f, 0.f, 1.f }
};

OpenGLMesh::OpenGLMesh() {
	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

void OpenGLMesh::Render(const Matrix4x4& mvp, Material* mat) {
	SF_ASSERT(dynamic_cast<OpenGLMaterial*>(mat) != nullptr, "Material passed in is either nullptr or is not a valid OpenGLMaterial")
	
	// test stuff starts here

	GLint mvp_location, vpos_location, vcol_location;

	GLuint program = reinterpret_cast<OpenGLMaterial*>(mat)->handle;

	mvp_location = glGetUniformLocation(program, "MVP");
	vpos_location = glGetAttribLocation(program, "vPos");
	vcol_location = glGetAttribLocation(program, "vCol");

	glEnableVertexAttribArray(vpos_location);
	glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,
	sizeof(vertices[0]), (void*)0);
	glEnableVertexAttribArray(vcol_location);
	glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
	sizeof(vertices[0]), (void*)(sizeof(float) * 2));

	float ratio = 640 / (float) 480;

	//mat4x4 m, p, mvp;
	//mat4x4_identity(m);
	//mat4x4_rotate_Z(m, m, (float)glfwGetTime());
	//mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
	//mat4x4_mul(mvp, p, m);
	// test stuff ends here

	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(reinterpret_cast<OpenGLMaterial*>(mat)->handle, "MVP"), 1, GL_FALSE, (const GLfloat*)&mvp);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

VertexBuffer& OpenGLMesh::GetVertexBuffer() {
	throw std::exception("OpenGLMesh::GetVertexBuffer has not been implemented");
}