#pragma once

#include <Rendering.h>
#include <stdint.h>
#include <MinimalCore.h>

enum class RENDERING_API ShaderType : uint8_t {
	None,
	Vertex,
	Geometry,
	Hull,
	Domain,
	Fragment,
	Compute
};

class RENDERING_API Shader {
protected:
	Shader() = delete;
	Shader(ShaderType type) : type(type) {};

public:
	virtual ~Shader() {};

	inline ShaderType GetShaderType() const {
		return type;
	}

	bool BuildShader(const char* str);

private:
	ShaderType type;
	bool isValid = false;

protected:
	virtual bool CompileShader(const char* str) = 0;

public:
	inline bool GetValid() const {
		return isValid;
	}
};