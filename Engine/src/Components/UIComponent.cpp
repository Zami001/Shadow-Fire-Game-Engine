#include <Components/UIComponent.h>
#include <Game.h>
#include <Application.h>
#include <UI/UIImage.h>

UIComponent::UIComponent() {}

UIComponent::~UIComponent() {}

void UIComponent::Tick(float DeltaTime) {
	UI->Render(Application::GetWindows()[0]->GetSize(), Application::GetWindows()[0]->GetSize());
}

void UIComponent::Initialize() {
	Component::Initialize();

	EnableTick();
	UI = new UIRoot(GetGameInstance()->GetRenderer());
	UI->AddElement<UIImage>();
}