#pragma once

#include <Font.h>
#include <iostream>
#include <RenderPipeline.h>
//#include <msdfgen.h>
//#include <msdfgen-ext.h>


#include <msdf-atlas-gen/msdf-atlas-gen.h>

//using namespace msdfgen;
using namespace msdf_atlas;

msdfgen::FreetypeHandle* Font::FreeType;

Font::Font(RenderPipeline& pipeline) : CharacterAtlas(pipeline.CreateTexture2D()), Geometry(nullptr), Glyphs(nullptr) {
}

Font::~Font() {
    delete Glyphs;
    delete Geometry;
}

void Font::LoadFontFile(const char* filename) {
    bool success = false;
    if (msdfgen::FontHandle* font = msdfgen::loadFont(FreeType, filename)) {
        delete Geometry;
        delete Glyphs;
        std::vector<GlyphGeometry>& glyphs = *new std::vector<GlyphGeometry>();
        FontGeometry& fontGeometry = *new FontGeometry(&glyphs);
        Glyphs = &glyphs;
        Geometry = &fontGeometry;

        Charset charset = Charset::ASCII;
        charset.add(L'Ý');
        fontGeometry.loadCharset(font, 2.0, charset);
        const double maxCornerAngle = 3.0;

        for (GlyphGeometry& glyph : glyphs) {
            glyph.edgeColoring(&msdfgen::edgeColoringInkTrap, maxCornerAngle, 0);
        }

        TightAtlasPacker packer;
        packer.setDimensionsConstraint(DimensionsConstraint::POWER_OF_TWO_SQUARE);
        packer.setMinimumScale(24.0);
        packer.setPixelRange(2.0);
        packer.setMiterLimit(1.0);
        packer.pack(glyphs.data(), glyphs.size());
        int width = 0, height = 0;
        packer.getDimensions(width, height);
        Scale = packer.getScale();
        ImmediateAtlasGenerator<float, 4, mtsdfGenerator, BitmapAtlasStorage<byte, 4>> generator(width, height);
        GeneratorAttributes attributes;
        generator.setAttributes(attributes);
        generator.setThreadCount(4);
        generator.generate(glyphs.data(), glyphs.size());
        
        CharacterAtlas->SubmitRawData(generator.atlasStorage().operator msdfgen::BitmapConstRef<byte, 4>().pixels, TextureFormat::R8G8B8A8_UNORM, width, height);

        msdfgen::destroyFont(font);
    }
}

void Font::InitializeFreeType() {
    if (FreeType) return;

    FreeType = msdfgen::initializeFreetype();
}

void Font::DeinitializeFreeType() {
    if (FreeType) {
        msdfgen::deinitializeFreetype(FreeType);
        FreeType = nullptr;
    }
}
