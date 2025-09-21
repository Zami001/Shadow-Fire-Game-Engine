#pragma once

#include <Rendering.h>
#include <UI/Panels/UIPanel.h>
#include <UI/UIImage.h>

class RENDERING_API UIButton : public UIPanel {
public:
	UIButton();
	~UIButton() = default;

	MulticastDelegate<> OnClick;

	virtual UIEventResponse OnMouseClick() override;

protected:
	virtual bool RequiresInput() const override { return true; }

	virtual void Render(Bounds2Di Bounds, Vector2i Screensize) override;
	virtual void Initialize() override;

private:
	UIImage* ButtonImage;
};