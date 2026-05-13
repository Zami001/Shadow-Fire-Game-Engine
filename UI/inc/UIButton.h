#pragma once

#include <UI.h>
#include <Panels/UIPanel.h>
#include <UIImage.h>

class UI_API UIButton : public UIPanel {
public:
	UIButton();
	~UIButton() = default;

	MulticastDelegate<> OnClick;

	virtual UIEventResponse OnMouseClick() override;

protected:
	virtual bool RequiresInput() const override { return true; }

	virtual void Render(Bounds2Di Bounds, Bounds2Di ClippingBounds, Vector2i Screensize) override;
	virtual void Initialize() override;

private:
	UIImage* ButtonImage;
};