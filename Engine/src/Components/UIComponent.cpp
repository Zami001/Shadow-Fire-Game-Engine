#include <Components/UIComponent.h>
#include <Game.h>
#include <Application.h>

// includes only for testing
#include <UI/UIImage.h>
#include <UI/UIText.h>

AssetType<UIComponent> assetType;

UIComponent::UIComponent() : buttonHandle(0), mouseHandle(0) {}

UIComponent::~UIComponent() {
	if (GetGameInstance()->GetWindows().size() == 0) return;

	GetGameInstance()->GetWindows()[0]->GetInputManager().OnButtonEvent.Remove(buttonHandle);
	GetGameInstance()->GetWindows()[0]->GetInputManager().OnMouseMoved.Remove(mouseHandle);
}

void UIComponent::Serialize(SerializedAsset& asset) {
	Component::Serialize(asset);

	asset.SetAssetType<UIComponent>();
}

void UIComponent::Tick(float DeltaTime) {
	UI->Render(GetGameInstance()->GetWindows()[0]->GetSize(), GetGameInstance()->GetWindows()[0]->GetSize());
}

void UIComponent::Initialize() {
	Component::Initialize();

	EnableTick();
	UI = new UIRoot(GetGameInstance()->GetRenderer());
	//UI->AddElement<UIImage>();
	auto text = UI->AddElement<UIText>();
	//text->SetText(L"(test|fg)Ý");
	text->SetText("This is a Test\r\nThis is also a test\r\nThis is another test");
	//text->SetText("Test");

	buttonHandle = GetGameInstance()->GetWindows()[0]->GetInputManager().OnButtonEvent.Add([this](Keycode Key, ButtonState State) { OnKeyEvent(Key, State); });
	mouseHandle = GetGameInstance()->GetWindows()[0]->GetInputManager().OnMouseMoved.Add([this](Vector2i MousePos) { OnMouseMove(MousePos); });
}

void UIComponent::OnMouseMove(Vector2i MousePos) {
	UI->OnMouseMove(MousePos);
}

void UIComponent::OnKeyEvent(Keycode key, ButtonState state) {
	UI->OnKeyEvent(key, state);
}