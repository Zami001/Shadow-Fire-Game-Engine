#pragma once

#include <Rendering.h>
#include <filesystem>
#include <MinimalCore.h>

class RENDERING_API Texture2D {
public:
	Texture2D() = default;
	virtual ~Texture2D() = default;

protected:
	uint32_t Width;
	uint32_t Height;

public:
	inline int GetWidth() const { return Width; }
	inline int GetHeight() const { return Height; }
	inline Vector2i GetSize() const { return { Width, Height }; }

	virtual void LoadFile(std::filesystem::path FilePath) = 0;
};