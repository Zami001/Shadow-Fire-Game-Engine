#pragma once

#include <UI/UIElement.h>
#include <Texture/Texture2D.h>

class RENDERING_API UIImage : public UIElement {
public:
	UIImage();
	~UIImage();

public:
	enum ScaleMode {
		Stretch,
		MaintainAspect,
		Crop,
		Repeat
	};

	void SetTexture(SFSharedPtr<Texture2D> Texture);
	inline void SetScaleMode(ScaleMode mode) {
		Scale = mode;
		MarkDirty();
	}

protected:
	virtual void Render(Bounds2Di Bounds, Vector2i Screensize) override;
	virtual void Initialize() override;
	virtual Vector2i GetDesiredSize() const override;

private:
	ScaleMode Scale = Crop;
};