#pragma once

#include <Components/Component.h>
#include <UI/Panels/UIRoot.h>

class ENGINE_API UIComponent : public Component {
public:
	UIComponent();
	~UIComponent();

protected:
	virtual void Tick(float DeltaTime) override;
	virtual void Initialize() override;

private:
	SFSharedPtr<UIRoot> UI;

	void OnMouseMove(Vector2i MousePos);
	void OnKeyEvent(Keycode key, ButtonState state);

	InputManager::Handle mouseHandle;
	InputManager::Handle buttonHandle;
};