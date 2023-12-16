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

public:
	template<typename T>
	inline std::enable_if_t<std::is_base_of_v<UIElement, T>, SFSharedRef<UIElement>> AddElement() {
		SFSharedRef<UIElement> element = new T();

		element->Parent = this;

		Subelements.push_back(element);
	}

	void RemoveElement(SFSharedPtr<UIElement> Element);
};