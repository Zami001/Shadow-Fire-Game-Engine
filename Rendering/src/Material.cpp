#include <Material.h>

bool Material::BindShaders(const std::vector<SFSharedRef<Shader>>& Shaders) {
	for (const SFSharedRef<Shader>& shader : Shaders) {
		if (!shader->GetValid()) {
			return false;
		}

		switch (shader->GetShaderType()) {
			case ShaderType::Vertex:
				vertex = shader;
				break;

			case ShaderType::Fragment:
				fragment = shader;
				break;

			default:
				SF_LOG(LogMaterial, Warning, "Only shader types of vertex and fragment are currently supported");
				break;
		}
	}

	isBound = SetupMaterial();
	return isBound;
}