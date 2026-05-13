#pragma once

#include <UI.h>
#include <Panels/UIPanel.h>

class UI_API ResizablePanel : public UIPanel {
public:
	enum SplitDirection {
		Horizontal,
		Vertical
	};

	ResizablePanel() = default;
	~ResizablePanel() = default;

	virtual bool RequiresInput() const override;
	virtual void OnMouseMove(Vector2i MousePos) override;
	virtual UIEventResponse OnMouseClick() override;

protected:
	virtual void Render(Bounds2Di Bounds, Bounds2Di ClippingBounds, Vector2i Screensize);

private:
	SplitDirection Split;

public:
	void SetSplitDirection(SplitDirection NewDirection);
	inline SplitDirection GetSplitDirection() const { return Split; }
};