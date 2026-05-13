#pragma once

#include <Panels/UIPanel.h>
#include <RenderPipeline.h>

class UI_API UIRoot : public UIPanel {
public:
	UIRoot(RenderPipeline& Pipeline);
	~UIRoot();

private:
	RenderPipeline& pipeline;

public:
	virtual Vector2i GetDesiredSize() const override;

	inline RenderPipeline& GetRenderPipeline() const { return pipeline; }

	virtual void Render(Bounds2Di Bounds, Bounds2Di ClippingBounds, Vector2i Screensize) override;
};