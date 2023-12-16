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

Camera cam;

void Game::Initialize() {
	GetRenderer().Init();
}

void Game::Step() {
	GetTickManager().Tick();

	for (int i = 0; i < Scenes.size(); ++i) {
		Scenes[i]->Render(cam);
	}
}

void Game::Shutdown() {
	Scenes.clear();

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