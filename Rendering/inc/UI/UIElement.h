#pragma once

#include <Rendering.h>
#include <MinimalCore.h>
#include <VertexBuffer.h>
#include <Material.h>

class UIPanel;

class RENDERING_API UIElement : public SharedFromThis<UIElement, false> {
public:
	friend UIPanel;

	UIElement() = default;
	virtual ~UIElement() = default;

protected:
	bool VisualElement = false;

private:
	SFWeakPtr<UIPanel> Parent;

public:
	inline bool IsVisualElement() const { return VisualElement; }
	virtual void Render(std::vector<Bounds2Di>& UI) = 0;

	void SetParent(SFWeakPtr<UIPanel> NewParent);
	inline SFWeakPtr<UIPanel> GetParent() const { return Parent; }

	virtual Vector2i GetDesiredSize() const = 0;
};