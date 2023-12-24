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

	UIElement() = default;
	virtual ~UIElement() = default;

	SFSharedPtr<Material> material;

protected:
	SFSharedPtr<Mesh> mesh;

	bool VisualElement = false;
	virtual void Render(Bounds2Di Bounds, Vector2i Screensize) = 0;
	virtual void ParentUpdated();
	virtual void Initialize() {};

	static Vector3 ScreenPosToScreenSpace(Vector2i position, Vector2i ScreenSize);

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