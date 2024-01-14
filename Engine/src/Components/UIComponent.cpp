#include <Components/UIComponent.h>
#include <Game.h>
#include <Application.h>
#include <UI/UIImage.h>

UIComponent::UIComponent() : buttonHandle(0), mouseHandle(0) {}

UIComponent::~UIComponent() {
	if (GetGameInstance()->GetWindows().size() == 0) return;

	GetGameInstance()->GetWindows()[0]->GetInputManager().UnregisterButtonListener(buttonHandle);
	GetGameInstance()->GetWindows()[0]->GetInputManager().UnregisterMouseListener(mouseHandle);
}

void UIComponent::Tick(float DeltaTime) {
	UI->Render(GetGameInstance()->GetWindows()[0]->GetSize(), GetGameInstance()->GetWindows()[0]->GetSize());
}

void UIComponent::Initialize() {
	Component::Initialize();

	EnableTick();
	UI = new UIRoot(GetGameInstance()->GetRenderer());
	UI->AddElement<UIImage>();

	buttonHandle = GetGameInstance()->GetWindows()[0]->GetInputManager().RegisterButtonListener([this](Keycode Key, ButtonState State) { OnKeyEvent(Key, State); });
	mouseHandle = GetGameInstance()->GetWindows()[0]->GetInputManager().RegisterMouseListener([this](Vector2i MousePos) { OnMouseMove(MousePos); });
}

void UIComponent::OnMouseMove(Vector2i MousePos) {
	UI->OnMouseMove(MousePos);
}

void UIComponent::OnKeyEvent(Keycode key, ButtonState state) {
	SF_LOG(LogTest, Log, "Key has been pressed: %i", static_cast<int>(key))
	UI->OnKeyEvent(key, state);
}