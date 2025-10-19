#pragma once

#include <Rendering.h>
#include <filesystem>
#include <MinimalCore.h>

enum class TextureFormat : uint8_t {
	Unknown,
	R32G32B32_FLOAT,
	R32G32B32A32_FLOAT,
	R8G8B8A8_UNORM
};

constexpr size_t BytesPerPixel(TextureFormat pixelFormat) {
	switch (pixelFormat) {
		case TextureFormat::R32G32B32A32_FLOAT: return 16;
		case TextureFormat::R32G32B32_FLOAT:	return 12;
		case TextureFormat::R8G8B8A8_UNORM:		return 4;
	}

	return 0;
}

class RENDERING_API Texture2D {
public:
	Texture2D() = default;
	virtual ~Texture2D() = default;

protected:
	uint32_t Width = 0;
	uint32_t Height = 0;

public:
	inline int GetWidth() const { return Width; }
	inline int GetHeight() const { return Height; }
	inline Vector2i GetSize() const { return { Width, Height }; }

	virtual void LoadFile(std::filesystem::path FilePath) = 0;
	virtual void SubmitRawData(const void* data, TextureFormat format, size_t width, size_t height) = 0;
};