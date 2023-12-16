#include <UI/UIElement.h>
#include <UI/Panels/UIPanel.h>

void UIElement::SetParent(SFWeakPtr<UIPanel> NewParent) {
	if (Parent == NewParent) {
		return;
	}

	// hold onto shared ref in case the old parent is the only reference holder
	SFSharedRef<UIElement> SharedThis = AsShared();

	if (Parent.Get()) {
		Parent->RemoveElement(this);
	}

	Parent = NewParent;
	if (Parent.Get()) {
		Parent->Subelements.push_back(SharedThis);
	}
}
