#include "Panels/ResizablePanel.h"

bool ResizablePanel::RequiresInput() const {
	return true;
}

void ResizablePanel::OnMouseMove(Vector2i MousePos) {
	UIPanel::OnMouseMove(MousePos);
}

UIEventResponse ResizablePanel::OnMouseClick() {
	return UIEventResponse();
}

void ResizablePanel::Render(Bounds2Di Bounds, Bounds2Di ClippingBounds, Vector2i Screensize) {

}

void ResizablePanel::SetSplitDirection(SplitDirection NewDirection) {
	Split = NewDirection;
	MarkDirty();
}
