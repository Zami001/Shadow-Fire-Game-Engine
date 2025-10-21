#pragma once

#include <Rendering.h>
#include <Texture/Texture2D.h>
#include <msdf-atlas-gen/msdf-atlas-gen.h>

namespace msdfgen {
	class FreetypeHandle;
}

class RENDERING_API Font {
	friend class RenderPipeline;
	friend class UIText;

public:
	Font() = delete;
	Font(RenderPipeline& pipeline);

	~Font();

	void LoadFontFile(const char* filename);
private:
	static msdfgen::FreetypeHandle* FreeType;

	SFSharedRef<Texture2D> CharacterAtlas;
	msdf_atlas::FontGeometry fontGeometry;
	std::vector<msdf_atlas::GlyphGeometry> glyphs;
	double Scale;

	static void InitializeFreeType();
	static void DeinitializeFreeType();

public:
	inline SFSharedRef<Texture2D> GetCharacterAtlas() const { return CharacterAtlas; }
};