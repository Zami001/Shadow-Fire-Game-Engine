#include <Game.h>

AssetType<Game> assetType;

Game::Game() {
	GameInstance = this;
}

Game::~Game() {}

void Game::Serialize(SerializedAsset& asset) {
	SFObject::Serialize(asset);
	
	asset.SetAssetType<Game>();
}

void Game::Initialize() {
	GetRenderer().Init();

	WindowParams winParams;
	winParams.VSync = false;
	//winParams.size = {2560, 1440};
	//winParams.style = WindowStyle::Borderless;
	Windows.push_back(GetRenderer().CreateWindow(winParams));

	if (audioEngine.InitAudio()) {
		audioEngine.StartAudio();
	}
}

void Game::Step() {
	GetTickManager().Tick();
	audioEngine.Update();

	for (int i = 0; i < Windows.size(); ++i) {
		Windows[i]->Present();
		Windows[i]->ProcessEvents();

		// close windows which need to be terminated and remove them from the list
		if (Windows[i]->GetShouldClose()) {
			Windows[i] = Windows[Windows.size() - 1];

			Windows.pop_back();
			--i;
		}
	}
}

void Game::Shutdown() {
	Scenes.clear();

	audioEngine.ShutdownAudio();
	GetRenderer().Shutdown();
}

SFWeakPtr<Scene> Game::CreateEmptyScene() {
	SFSharedRef<Scene> scene = NewObject<Scene>();
	Scenes.push_back(scene);
	return scene;
}

void Game::CreateInitialScene() {
	CreateEmptyScene();
}

std::vector<SFSharedPtr<Window>> Game::GetWindows() {
	return Windows;
}
