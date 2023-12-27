#include <UI/UIElement.h>
#include <UI/Panels/UIPanel.h>

void UIElement::ParentUpdated() {
	ParentRoot = Parent ? Parent->GetRoot() : nullptr;
}

Vector3 UIElement::ScreenPosToScreenSpace(Vector2i position, Vector2i ScreenSize) {
	float x = static_cast<float>(position.x * 2) / (ScreenSize.x) - 1;
	float y = static_cast<float>(position.y * 2) / (ScreenSize.y) - 1;
	return Vector3(x, -y, 0);
}

struct Vertex {
	Vector3 position;
	Vector2 UV;
};

void UIElement::SetBoxRender(Bounds2Di ScreenPos, Vector2i ScreenSize, Vector2 UVFrom, Vector2 UVTo, VertexBuffer& Buffer) {
	Vertex* verts = new Vertex[4];

	for (int i = 0; i < 4; ++i) {
		verts[i].UV.x = (i & 1) ? UVTo.x : UVFrom.x;
		verts[i].UV.y = (i & 2) ? UVTo.y : UVFrom.y;
		int x, y;
		x = (i % 2) ? (ScreenPos.position.x + ScreenPos.size.x) : ScreenPos.position.x;
		y = (i / 2) ? ScreenPos.position.y : (ScreenPos.position.y + ScreenPos.size.y);
		verts[i].position = ScreenPosToScreenSpace(Vector2i(x, y), ScreenSize);
	}

	Buffer.SetVertexData(verts, 4);

	VertexBuffer::IndexType* indices = new VertexBuffer::IndexType[6];
	indices[0] = 2;
	indices[1] = 1;
	indices[2] = 0;
	indices[3] = 1;
	indices[4] = 2;
	indices[5] = 3;

	Buffer.SetIndices(indices, 6);

	delete[] verts;
	delete[] indices;
}

void UIElement::SetSlicedBoxRender(Bounds2Di ScreenPos, Vector2i ScreenSize, SlicedUI slices, Vector2i OriginalSize, VertexBuffer& Buffer) {
	Vertex* verts = new Vertex[16];

	int xTotalSlice = slices.Left + slices.Right;
	int yTotalSlice = slices.Top + slices.Bottom;

	int LeftOffset = xTotalSlice <= ScreenPos.size.x ? slices.Left : (static_cast<float>(slices.Left) / static_cast<float>(xTotalSlice)) * ScreenPos.size.x;
	int RightOffset = xTotalSlice <= ScreenPos.size.x ? slices.Right : (static_cast<float>(slices.Right) / static_cast<float>(xTotalSlice)) * ScreenPos.size.x;
	int TopOffset = yTotalSlice <= ScreenPos.size.y ? slices.Top : (static_cast<float>(slices.Top) / static_cast<float>(yTotalSlice)) * ScreenPos.size.y;
	int BottomOffset = yTotalSlice <= ScreenPos.size.y ? slices.Bottom : (static_cast<float>(slices.Bottom) / static_cast<float>(yTotalSlice)) * ScreenPos.size.y;

	float LeftUV = static_cast<float>(slices.Left) / static_cast<float>(OriginalSize.x);
	float RightUV = 1.f - (static_cast<float>(slices.Right) / static_cast<float>(OriginalSize.x));
	float TopUV = 1.f - (static_cast<float>(slices.Top) / static_cast<float>(OriginalSize.y));
	float BottomUV = static_cast<float>(slices.Bottom) / static_cast<float>(OriginalSize.y);

	verts[0].position = ScreenPosToScreenSpace(Vector2i(ScreenPos.position.x, ScreenPos.position.y), ScreenSize);
	verts[0].UV = { 0.f, 1.f };
	verts[1].position = ScreenPosToScreenSpace(Vector2i(ScreenPos.position.x + ScreenPos.size.x, ScreenPos.position.y), ScreenSize);
	verts[1].UV = { 1.f, 1.f };
	verts[2].position = ScreenPosToScreenSpace(Vector2i(ScreenPos.position.x + ScreenPos.size.x, ScreenPos.position.y + ScreenPos.size.y), ScreenSize);
	verts[2].UV = { 1.f, 0.f };
	verts[3].position = ScreenPosToScreenSpace(Vector2i(ScreenPos.position.x, ScreenPos.position.y + ScreenPos.size.y), ScreenSize);
	verts[3].UV = { 0.f, 0.f };
	verts[4].position = ScreenPosToScreenSpace(Vector2i(ScreenPos.position.x + LeftOffset, ScreenPos.position.y + TopOffset), ScreenSize);
	verts[4].UV = { LeftUV, TopUV };
	verts[5].position = ScreenPosToScreenSpace(Vector2i(ScreenPos.position.x + ScreenPos.size.x - RightOffset, ScreenPos.position.y + TopOffset), ScreenSize);
	verts[5].UV = { RightUV, TopUV };
	verts[6].position = ScreenPosToScreenSpace(Vector2i(ScreenPos.position.x + ScreenPos.size.x - RightOffset, ScreenPos.position.y + ScreenPos.size.y - BottomOffset), ScreenSize);
	verts[6].UV = { RightUV, BottomUV };
	verts[7].position = ScreenPosToScreenSpace(Vector2i(ScreenPos.position.x + LeftOffset, ScreenPos.position.y + ScreenPos.size.y - BottomOffset), ScreenSize);
	verts[7].UV = { LeftUV, BottomUV };
	verts[8].position = ScreenPosToScreenSpace(Vector2i(ScreenPos.position.x + LeftOffset, ScreenPos.position.y), ScreenSize);
	verts[8].UV = { LeftUV, 1.f };
	verts[9].position = ScreenPosToScreenSpace(Vector2i(ScreenPos.position.x - RightOffset + ScreenPos.size.x, ScreenPos.position.y), ScreenSize);
	verts[9].UV = { RightUV, 1.f };
	verts[10].position = ScreenPosToScreenSpace(Vector2i(ScreenPos.position.x + ScreenPos.size.x, ScreenPos.position.y + TopOffset), ScreenSize);
	verts[10].UV = { 1.f, TopUV };
	verts[11].position = ScreenPosToScreenSpace(Vector2i(ScreenPos.position.x + ScreenPos.size.x, ScreenPos.position.y + ScreenPos.size.y - BottomOffset), ScreenSize);
	verts[11].UV = { 1.f, BottomUV };
	verts[12].position = ScreenPosToScreenSpace(Vector2i(ScreenPos.position.x + ScreenPos.size.x - RightOffset, ScreenPos.position.y + ScreenPos.size.y), ScreenSize);
	verts[12].UV = { RightUV, 0.f };
	verts[13].position = ScreenPosToScreenSpace(Vector2i(ScreenPos.position.x + LeftOffset, ScreenPos.position.y + ScreenPos.size.y), ScreenSize);
	verts[13].UV = { LeftUV, 0.f };
	verts[14].position = ScreenPosToScreenSpace(Vector2i(ScreenPos.position.x, ScreenPos.position.y + ScreenPos.size.y - BottomOffset), ScreenSize);
	verts[14].UV = { 0.f, BottomUV };
	verts[15].position = ScreenPosToScreenSpace(Vector2i(ScreenPos.position.x, ScreenPos.position.y + TopOffset), ScreenSize);
	verts[15].UV = { 0.f, TopUV };

	Buffer.SetVertexData(verts, 16);

	VertexBuffer::IndexType* indices = new VertexBuffer::IndexType[54]{ 
		0, 8, 4, 0, 4, 15, // top-left corner
		8, 9, 5, 5, 4, 8, // top line
		1, 5, 9, 5, 1, 10, // top right corner
		10, 6, 5, 6, 10, 11, // right line
		11, 2, 6, 6, 2, 12, // bottom right corner
		6, 12, 13, 13, 7, 6, // bottom line
		7, 13, 3, 7, 3, 14, // bottom left corner
		7, 14, 15, 7, 15, 4, // left line
		4, 5, 6, 7, 4, 6 // inner area
	};

	Buffer.SetIndices(indices, 54);

	delete[] verts;
	delete[] indices;
}

void UIElement::SetParent(SFWeakPtr<UIPanel> NewParent) {
	if (Parent == NewParent) {
		return;
	}

	// hold onto shared ref in case the old parent is the only reference holder
	SFSharedRef<UIElement> SharedThis = AsShared();

	if (Parent) {
		Parent->RemoveElement(this);
	}

	Parent = NewParent;
	if (Parent) {
		Parent->Subelements.push_back(SharedThis);
	}

	ParentUpdated();
}

void UIElement::MarkDirty() {
	Dirty = true;
}

void UIElement::ClearDirty() {
	Dirty = false;
}