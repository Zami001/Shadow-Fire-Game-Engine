#pragma once

#include <Panels/UIPanel.h>

class UI_API OverlayPanel : public UIPanel {
public:
	OverlayPanel() = default;
	~OverlayPanel() = default;

protected:
	virtual void Render(Bounds2Di Bounds, Bounds2Di ClippingBounds, Vector2i Screensize) override;
};