#include <UI/Panels/UIPanel.h>

void UIPanel::ParentUpdated() {
	UIElement::ParentUpdated();

	for (int i = 0; i < Subelements.size(); ++i) {
		Subelements[i]->ParentRoot = ParentRoot;
		Subelements[i]->ParentUpdated();
	}
}

void UIPanel::RemoveElement(SFSharedPtr<UIElement> Element) {
	if (Element.Get()) {
		for (int i = 0; i < Subelements.size(); ++i) {
			if (Subelements[i].Get() == Element.Get()) {
				Subelements.erase(Subelements.begin() + i);
				return;
			}
		}
	}
}

void UIPanel::MarkDirty() {
	UIElement::MarkDirty();

	for (int i = 0; i < Subelements.size(); ++i) {
		Subelements[i]->MarkDirty();
	}
}