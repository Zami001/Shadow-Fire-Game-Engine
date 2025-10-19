#pragma once

#include <Rendering.h>
#include <Texture/Texture2D.h>

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
	void* Geometry;
	void* Glyphs;
	double Scale;

	static void InitializeFreeType();
	static void DeinitializeFreeType();

public:
	inline SFSharedRef<Texture2D> GetCharacterAtlas() const { return CharacterAtlas; }
};