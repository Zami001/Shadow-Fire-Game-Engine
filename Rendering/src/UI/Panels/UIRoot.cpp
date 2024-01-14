#include <UI/Panels/UIRoot.h>

UIRoot::UIRoot(RenderPipeline& Pipeline) : pipeline(Pipeline) {
	ParentRoot = this;
}

UIRoot::~UIRoot() {
}

Vector2i UIRoot::GetDesiredSize() const {
	return Vector2i();
}

void UIRoot::Render(Bounds2Di Bounds, Vector2i Screensize) {
	Bounds.size = Bounds.size * 0.5f;
	for (int i = 0; i < Subelements.size(); ++i) {
		Subelements[i]->Render(Bounds, Screensize);
		Subelements[i]->LastBounds = Bounds;
	}
}