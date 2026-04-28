#pragma once

#include <UIElement.h>
#include <Texture/Texture2D.h>
#include <Font.h>

class UI_API UIText : public UIElement {
public:
	UIText();
	~UIText();

public:
	void SetText(const std::wstring& newText);
	void SetText(const std::string& newText);
	std::wstring GetText() const;

	int GetCharacterWidth(wchar_t character) const;
	Vector2i GetTextSize(const std::wstring& str) const;
	double GetTextWidth(const std::wstring& str) const;
	double GetTextWidth() const;
	size_t CountLines(const std::wstring& str) const;

protected:
	virtual void Render(Bounds2Di Bounds, Vector2i Screensize) override;
	virtual void Initialize() override;
	virtual Vector2i GetDesiredSize() const override;

private:
	SFSharedPtr<Font> font;
	std::wstring text;

	void SetupVertexBuffer(Bounds2Di Bounds, Vector2i Screensize);
	Vector2i SetupLetterVertex(void* verticies, wchar_t letter, wchar_t prevLetter, Vector2i prevPosition, Vector2i ScreenSize);
};