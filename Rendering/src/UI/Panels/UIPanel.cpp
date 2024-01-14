#include <UI/Panels/UIPanel.h>

void UIPanel::ParentUpdated() {
	UIElement::ParentUpdated();

	for (int i = 0; i < Subelements.size(); ++i) {
		Subelements[i]->ParentRoot = ParentRoot;
		Subelements[i]->ParentUpdated();
	}
}

void UIPanel::OnMouseMove(Vector2i MousePos) {
	UIElement::OnMouseMove(MousePos);

	for (int i = 0; i < Subelements.size(); ++i) {
		if (Subelements[i]->RequiresInput()) {
			Subelements[i]->OnMouseMove(MousePos);
		}
	}
}

UIEventResponse UIPanel::OnMouseClick() {
	UIEventResponse response;
	
	for (int i = 0; i < Subelements.size(); ++i) {
		if (Subelements[i]->RequiresInput() && Subelements[i]->IsMouseHovered()) {
			if (Subelements[i]->OnMouseClick() == UIEventResponse::Handled) {
				return UIEventResponse::Handled;
			}
		}
	}
	
	return UIEventResponse::Unhandled;
}

UIEventResponse UIPanel::OnKeyEvent(Keycode key, ButtonState state) {
	UIEventResponse response = UIEventResponse::Unhandled;

	for (int i = 0; i < Subelements.size(); ++i) {
		if (Subelements[i]->RequiresInput()) {
			response = static_cast<UIEventResponse>(static_cast<int>(response) | static_cast<int>(Subelements[i]->OnKeyEvent(key, state)));
		}
	}

	return UIElement::OnKeyEvent(key, state);
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