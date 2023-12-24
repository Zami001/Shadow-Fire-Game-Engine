#include <RenderPipeline.h>
#include <ShaderImporter.h>

void RenderPipeline::Init() {
	
}

void RenderPipeline::Shutdown() {
	
}

SFSharedRef<Shader> RenderPipeline::GetShader(std::filesystem::path Filename, ShaderType Type) {
	if (!Filename.is_absolute()) {
		Filename = std::filesystem::absolute(Filename);
	}

	auto cached = ShaderCache.find(Filename);
	if (cached != ShaderCache.end() && cached->second) {
		return cached->second;
	}

	SFSharedRef<Shader> shader = CreateShader(Type);
	ShaderImporter importer;
	importer.ImportFile(Filename, *shader);

	ShaderCache.insert({ Filename, shader });
	return shader;
}

SFSharedRef<Texture2D> RenderPipeline::GetTexture2D(std::filesystem::path Filename) {
	if (!Filename.is_absolute()) {
		Filename = std::filesystem::absolute(Filename);
	}

	auto cached = Texture2DCache.find(Filename);
	if (cached != Texture2DCache.end() && cached->second) {
		return cached->second;
	}

	SFSharedRef<Texture2D> texture = CreateTexture2D();
	texture->LoadFile(Filename);

	Texture2DCache.insert({ Filename, texture });

	return texture;
}