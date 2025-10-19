#include <UI/UIText.h>
#include <UI/Panels/UIRoot.h>
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
		//newString[i] = newText[i];
	}

	text = std::move(newString);
	MarkDirty();
}

std::wstring UIText::GetText() const {
	return text;
}

void UIText::Render(Bounds2Di Bounds, Vector2i Screensize) {
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

	mesh->GetVertexBuffer().SetDescriptors({
		VertexBuffer::VertexDescriptor::Position,
		VertexBuffer::VertexDescriptor::UV_Coordinate
	});

	font = new Font(GetRoot()->GetRenderPipeline());
	font->LoadFontFile("C:\\Windows\\Fonts\\arial.ttf");
	material->Textures.push_back(font->GetCharacterAtlas());
}

Vector2i UIText::GetDesiredSize() const {
	return Vector2i(400, 100);
}

struct Vertex {
	Vector3 position;
	Vector2 UV;
};

void UIText::SetupVertexBuffer(Bounds2Di Bounds, Vector2i Screensize) {
	// find whitespaces that arn't included in rendered geometry
	msdf_atlas::FontGeometry& geometry = *reinterpret_cast<msdf_atlas::FontGeometry*>(font->Geometry);
	size_t whiteSpaces = 0;
	for (int i = 0; i < text.size(); ++i) {
		if (geometry.getGlyph(text[i])->isWhitespace()) {
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
	msdf_atlas::FontGeometry& geometry = *reinterpret_cast<msdf_atlas::FontGeometry*>(font->Geometry);
	Vector2i pos = prevPosition;
	double ascender = geometry.getMetrics().ascenderY * font->Scale;
	double decender = geometry.getMetrics().descenderY * font->Scale;
	double emSize = geometry.getMetrics().emSize * font->Scale;
	double lineHeight = geometry.getMetrics().lineHeight * font->Scale;

	double advance = 0;
	if (prevLetter != L'\0') {
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

	for (int i = 0; i < 4; ++i) {
		verts[i].UV.x = (i & 1) ? UVTo.x : UVFrom.x;
		verts[i].UV.y = (i & 2) ? UVFrom.y : UVTo.y;
		int x, y;
		x = (i % 2) ? pos.x + toPos.x : pos.x + fromPos.x;
		y = ((i / 2) ? pos.y - fromPos.y : (pos.y - toPos.y)) + ascender;
		verts[i].position = ScreenPosToScreenSpace(Vector2i(x, y), ScreenSize);
	}

	return pos;
}