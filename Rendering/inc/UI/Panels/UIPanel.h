#pragma once

#include <UI/UIElement.h>
#include <vector>
#include <MinimalCore.h>

class RENDERING_API UIPanel : public UIElement {
	friend void UIElement::SetParent(SFWeakPtr<UIPanel>);

public:
	UIPanel() = default;
	~UIPanel() = default;

protected:
	std::vector<SFSharedRef<UIElement>> Subelements;
	virtual void ParentUpdated() override;
	void RenderSubelement(SFSharedRef<UIElement>& Element, Bounds2Di Bounds, Vector2i Screensize);

public:
	virtual bool RequiresInput() const override;
	virtual void OnMouseMove(Vector2i MousePos) override;
	virtual UIEventResponse OnMouseClick() override;
	virtual UIEventResponse OnKeyEvent(Keycode key, ButtonState state) override;

	template<typename T>
	inline std::enable_if_t<std::is_base_of_v<UIElement, T>, SFSharedRef<UIElement>> AddElement() {
		SFSharedRef<UIElement> element = new T();

		element->Parent = this;
		element->ParentRoot = GetRoot();

		Subelements.push_back(element);

		element->Initialize();
		return element;
	}

	void RemoveElement(SFSharedPtr<UIElement> Element);

	virtual void MarkDirty() override;
};