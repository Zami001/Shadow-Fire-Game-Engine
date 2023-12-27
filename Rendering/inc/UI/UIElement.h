#pragma once

#include <Rendering.h>
#include <MinimalCore.h>
#include <VertexBuffer.h>
#include <Material.h>
#include <Mesh.h>

class UIPanel;
class UIRoot;

class RENDERING_API UIElement : public SharedFromThis<UIElement, false> {
public:
	friend UIPanel;
	friend UIRoot;

	struct SlicedUI {
		int Left = 0;
		int Top = 0;
		int Right = 0;
		int Bottom = 0;
	};

	UIElement() = default;
	virtual ~UIElement() = default;

protected:
	SFSharedPtr<Mesh> mesh;
	SFSharedPtr<Material> material;

	bool VisualElement = false;
	virtual void Render(Bounds2Di Bounds, Vector2i Screensize) = 0;
	virtual void ParentUpdated();
	virtual void Initialize() {};

	static Vector3 ScreenPosToScreenSpace(Vector2i position, Vector2i ScreenSize);
	static void SetBoxRender(Bounds2Di ScreenPos, Vector2i ScreenSize, Vector2 UVFrom, Vector2 UVTo, VertexBuffer& Buffer);
	static void SetSlicedBoxRender(Bounds2Di ScreenPos, Vector2i ScreenSize, SlicedUI Slices, Vector2i OriginalSize, VertexBuffer& Buffer);

private:
	UIPanel* Parent = nullptr;
	UIRoot* ParentRoot = nullptr;
	bool Dirty = true;

public:
	inline bool IsVisualElement() const { return VisualElement; }

	void SetParent(SFWeakPtr<UIPanel> NewParent);
	inline UIPanel* GetParent() const { return Parent; }
	inline UIRoot* GetRoot() const { return ParentRoot; }

	virtual Vector2i GetDesiredSize() const = 0;

	inline bool IsDirty() const { return Dirty; }
	virtual void MarkDirty();

protected:
	void ClearDirty();
};