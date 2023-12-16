#pragma once

#include <OpenGLRenderer.h>
#include <Mesh.h>

class OPENGL_API OpenGLMesh : public Mesh {
public:
	OpenGLMesh();

public:
	unsigned int vertex_buffer;

	virtual void Render(const Matrix4x4& mvp, Material* mat) override;
	virtual VertexBuffer& GetVertexBuffer() override;
};