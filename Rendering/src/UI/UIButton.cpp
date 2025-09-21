#include <UI/UIButton.h>
#include <UI/UIImage.h>

UIButton::UIButton() {
}

UIEventResponse UIButton::OnMouseClick() {
	UIPanel::OnMouseClick();

	OnClick();
	return UIEventResponse::Handled;
}

void UIButton::Render(Bounds2Di Bounds, Vector2i Screensize) {
	for (int i = 0; i < Subelements.size(); ++i) {
		RenderSubelement(Subelements[i], Bounds, Screensize);
	}
}

void UIButton::Initialize() {
	UIPanel::Initialize();

	ButtonImage = static_cast<UIImage*>(AddElement<UIImage>().Get());
}
