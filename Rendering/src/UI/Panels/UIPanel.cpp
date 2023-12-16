#include <UI/Panels/UIPanel.h>

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
