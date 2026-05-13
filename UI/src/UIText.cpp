#include <UIText.h>
#include <Panels/UIRoot.h>
#include <msdf-atlas-gen/msdf-atlas-gen.h>

#include <cstdlib>

UIText::UIText() {}

UIText::~UIText() {}

void UIText::SetText(const std::wstring& newText) {
	text = newText;
	MarkDirty();
}

void UIText::SetText(const std::string& newText) {
	std::wstring newString;
	newString.reserve(newText.size());

	for (int i = 0; i < newText.size(); ++i) {
		newString.push_back(newText[i]);
	}

	text = std::move(newString);
	MarkDirty();
}

std::wstring UIText::GetText() const {
	return text;
}

int UIText::GetCharacterWidth(wchar_t character) const {
	double x, y, w, h;
	font->fontGeometry.getGlyph(character)->getQuadPlaneBounds(x, y, w, h);
	return (w) * font->Scale;
}

Vector2i UIText::GetTextSize(const std::wstring& str) const {
	double longestLine = 0;
	size_t width = 0;
	size_t lines = 1;

	for (int i = 1; i < str.size(); ++i) {
		if (str[i] == L'\r' || str[i] == L'\n') {
			width += GetCharacterWidth(str[i - 1]);
			longestLine = longestLine > width ? longestLine : width;
			width = 0;

			lines++;

			if (str.size() > i + 1 && str[i] == L'\r' && str[i + 1] == L'\n') {
				++i;
			}
			continue;
		}

		double increment;
		font->fontGeometry.getAdvance(increment, str[i - 1], str[i]);
		width += increment * font->Scale;
	}

	wchar_t lastChar = str[str.size() - 1];
	if (lastChar != L'\r' && lastChar != L'\n') {
		width += GetCharacterWidth(lastChar);
	}

	return Vector2i((longestLine > width ? longestLine : width), font->fontGeometry.getMetrics().lineHeight * lines * font->Scale);
}

double UIText::GetTextWidth(const std::wstring& str) const {
	return GetTextSize(str).x;
}

double UIText::GetTextWidth() const {
	if (!IsDirty()) {
		return LastBounds.size.x;
	}

	return GetTextWidth(text);
}

size_t UIText::CountLines(const std::wstring& str) const {
	size_t lines = 1;
	for (int i = 0; i < str.size(); ++i) {
		if (str[i] == L'\r') {
			lines++;

			// treat \r\n as a single new line
			if (str.size() > i + 1 && str[i + 1] == L'\n') {
				++i;
				continue;
			}
		}

		if (str[i] == L'\n') {
			lines++;
		}
	}

	return lines;
}

void UIText::Render(Bounds2Di Bounds, Bounds2Di ClippingBounds, Vector2i Screensize) {
	// todo: needs to work correctly with clipping bounds
	if (IsDirty()) {
		SetupVertexBuffer(Bounds, Screensize);
		ClearDirty();
	}

	mesh->Render(Matrix4x4::Identity(), material);
}

void UIText::Initialize() {
	UIElement::Initialize();

	mesh = GetRoot()->GetRenderPipeline().CreateMesh();
	material = GetRoot()->GetRenderPipeline().CreateMaterial();

	SFSharedRef<Shader> VertShader = GetRoot()->GetRenderPipeline().GetShader("Shaders/Text/Vert.hlsl", ShaderType::Vertex);
	SFSharedRef<Shader> FragShader = GetRoot()->GetRenderPipeline().GetShader("Shaders/Text/Frag.hlsl", ShaderType::Fragment);

	material->BindShaders({ VertShader, FragShader });
	material->pass = RenderPass::GUI;

	mesh->GetVertexBuffer().SetDescriptors({
		VertexBuffer::VertexDescriptor::Position,
		VertexBuffer::VertexDescriptor::UV_Coordinate
	});

	font = new Font(GetRoot()->GetRenderPipeline());
	font->LoadFontFile("C:\\Windows\\Fonts\\arial.ttf");
	material->Textures.push_back(font->GetCharacterAtlas());
}

Vector2i UIText::GetDesiredSize() const {
	//if (!IsDirty()) {
	//	return LastBounds.size;
	//}

	return GetTextSize(text);
}

struct Vertex {
	Vector3 position;
	Vector2 UV;
};

void UIText::SetupVertexBuffer(Bounds2Di Bounds, Vector2i Screensize) {
	// find whitespaces that arn't included in rendered geometry
	msdf_atlas::FontGeometry& geometry = font->fontGeometry;
	size_t whiteSpaces = 0;
	for (int i = 0; i < text.size(); ++i) {
		if (text[i] == L'\r' || text[i] == L'\n' || geometry.getGlyph(text[i])->isWhitespace()) {
			whiteSpaces++;
		}
	}

	size_t renderedCharacters = text.size() - whiteSpaces;
	size_t numVerts = 4 * renderedCharacters;
	size_t numIndices = 6 * renderedCharacters;

	Vertex* verts = new Vertex[numVerts];

	VertexBuffer& Buffer = mesh->GetVertexBuffer();
	Buffer.SetDescriptors({ 
		VertexBuffer::VertexDescriptor::Position,
		VertexBuffer::VertexDescriptor::UV_Coordinate
	});

	Vector2i pos = Bounds.position;
	wchar_t prevLetter = L'\0';
	Vertex* vertInsert = verts;

	for (int i = 0; i < text.size(); ++i) {
		// account for newlines
		if (text[i] == L'\r' || text[i] == L'\n') {
			pos.x = Bounds.position.x;
			pos.y += font->fontGeometry.getMetrics().lineHeight * font->Scale;

			// treat \r\n as single newline
			if (text.size() > i + 1 && text[i] == L'\r' && text[i + 1] == L'\n') {
				i++;
			}

			prevLetter = text[i];
			continue;
		}

		// increment whitespace positioning but not vertex count
		if (geometry.getGlyph(text[i])->isWhitespace()) {
			double advance = 0;
			if (prevLetter != L'\0') {
				geometry.getAdvance(advance, prevLetter, text[i]);
			}

			pos.x += advance * font->Scale;
			prevLetter = text[i];
			continue;
		}

		// continue with rendered characters
		pos = SetupLetterVertex(vertInsert, text[i], prevLetter, pos, Screensize);
		prevLetter = text[i];
		vertInsert += 4;
	}

	Buffer.SetVertexData(verts, numVerts);

	VertexBuffer::IndexType* indices = new VertexBuffer::IndexType[numIndices];
	for (int i = 0; i < renderedCharacters; ++i) {
		size_t indexOffset = i * 6;
		size_t vertexOffset = i * 4;

		indices[0 + indexOffset] = 0 + vertexOffset;
		indices[1 + indexOffset] = 1 + vertexOffset;
		indices[2 + indexOffset] = 2 + vertexOffset;
		indices[3 + indexOffset] = 3 + vertexOffset;
		indices[4 + indexOffset] = 2 + vertexOffset;
		indices[5 + indexOffset] = 1 + vertexOffset;
	}

	Buffer.SetIndices(indices, numIndices);

	delete[] verts;
	delete[] indices;
}

Vector2i UIText::SetupLetterVertex(void* verticies, wchar_t letter, wchar_t prevLetter, Vector2i prevPosition, Vector2i ScreenSize) {
	Vertex* verts = reinterpret_cast<Vertex*>(verticies);
	msdf_atlas::FontGeometry& geometry = font->fontGeometry;
	Vector2i pos = prevPosition;
	double ascender = geometry.getMetrics().ascenderY * font->Scale;
	double decender = geometry.getMetrics().descenderY * font->Scale;
	double emSize = geometry.getMetrics().emSize * font->Scale;
	double lineHeight = geometry.getMetrics().lineHeight * font->Scale;

	double advance = 0;
	if (prevLetter != L'\0' && prevLetter != L'\r' && prevLetter != L'\n') {
		geometry.getAdvance(advance, prevLetter, letter);
		pos.x += advance * font->Scale;
	}

	Vector2 UVFrom, UVTo;
	Vector2i FromInt, ToInt;
	geometry.getGlyph(letter)->getBoxRect(FromInt.x, FromInt.y, ToInt.x, ToInt.y);
	ToInt = FromInt + ToInt;
	UVFrom.x = static_cast<float>(FromInt.x) / font->CharacterAtlas->GetWidth();
	UVFrom.y = static_cast<float>(FromInt.y) / font->CharacterAtlas->GetHeight();
	UVTo.x = static_cast<float>(ToInt.x) / font->CharacterAtlas->GetWidth();
	UVTo.y = static_cast<float>(ToInt.y) / font->CharacterAtlas->GetHeight();

	Vector<double, 2> fromPos;
	Vector<double, 2> toPos;
	geometry.getGlyph(letter)->getQuadPlaneBounds(fromPos.x, fromPos.y, toPos.x, toPos.y);
	fromPos = fromPos * font->Scale;
	toPos = toPos * font->Scale;

	float maxWidth = 0;

	for (int i = 0; i < 4; ++i) {
		verts[i].UV.x = (i & 1) ? UVTo.x : UVFrom.x;
		verts[i].UV.y = (i & 2) ? UVFrom.y : UVTo.y;
		int x, y;
		x = (i % 2) ? pos.x + toPos.x : pos.x + fromPos.x;
		y = ((i / 2) ? pos.y - fromPos.y : (pos.y - toPos.y)) + ascender;
		verts[i].position = ScreenPosToScreenSpace(Vector2i(x, y), ScreenSize);
		if (x > maxWidth) {
			maxWidth = x;
		}
	}

	return pos;
}