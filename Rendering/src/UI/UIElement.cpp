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