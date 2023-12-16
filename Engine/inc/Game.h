#pragma once

#include <Engine.h>
#include <RenderPipeline.h>
#include <Scene.h>
#include <TickManager.h>
#include <MinimalCore.h>

class ENGINE_API Game : public SFObject {
protected:
	Game();

public:
	~Game();

	virtual void Serialize(SerializedAsset& asset) override;

	virtual void Initialize();
	virtual void Step();
	virtual void Shutdown();

	virtual RenderPipeline& GetRenderer() = 0;

	SFWeakPtr<Scene> CreateEmptyScene();

	virtual void CreateInitialScene();

private:
	TickManager tickManager;
	std::vector<SFSharedRef<Scene>> Scenes;

public:
	inline TickManager& GetTickManager() {
		return tickManager;
	}

	inline const std::vector<SFSharedRef<Scene>>& GetScenes() {
		return Scenes;
	}
};