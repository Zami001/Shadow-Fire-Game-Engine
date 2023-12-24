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
	for (int i = 0; i < Subelements.size(); ++i) {
		Subelements[i]->Render(Bounds, Screensize);
	}
}
