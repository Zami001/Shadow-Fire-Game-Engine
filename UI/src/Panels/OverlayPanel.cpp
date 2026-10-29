#include <Panels/OverlayPanel.h>

void OverlayPanel::Render(Bounds2Di Bounds, Bounds2Di ClippingBounds, Vector2i Screensize) {
	for (int i = 0; i < Subelements.size(); ++i) {
		RenderSubelement(Subelements[i], Bounds, ClippingBounds, Screensize);
	}
}