#pragma once

#include <Rendering.h>
#include <MinimalCore.h>
#include <FileImporter.h>
#include <Shader.h>
#include <RenderPipeline.h>

class RENDERING_API ShaderImporter : public FileImporter<Shader> {
public:
	ShaderImporter();
	~ShaderImporter() = default;

protected:
	virtual bool ConvertData(Shader& Output, const char* data, size_t dataLength) override;
};