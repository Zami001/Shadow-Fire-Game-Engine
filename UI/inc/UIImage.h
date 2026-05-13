#pragma once

#include <UIElement.h>
#include <Texture/Texture2D.h>

class UI_API UIImage : public UIElement {
public:
	UIImage();
	~UIImage();

public:
	enum ScaleMode {
		Stretch,
		MaintainAspect,
		Crop,
		Repeat,
		Sliced
	};

	void SetTexture(SFSharedPtr<Texture2D> Texture);
	inline void SetScaleMode(ScaleMode mode) {
		Scale = mode;
		MarkDirty();
	}
	
	virtual void OnMouseOver() override;
	virtual bool RequiresInput() const override { return true; }

protected:
	virtual void Render(Bounds2Di Bounds, Bounds2Di ClippingBounds, Vector2i Screensize) override;
	virtual void Initialize() override;
	virtual Vector2i GetDesiredSize() const override;

private:
	ScaleMode Scale = Stretch;

	SlicedUI Slices = { 600, 150, 600, 150 };
};