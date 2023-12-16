#pragma once

#include <Editor.h>
#include <Game.h>
#include <vector>
#include <DX11Pipeline.h>
#include <Project.h>

class EditorGame : public Game {
public:
	EditorGame() = default;
	~EditorGame() = default;

	std::vector<Game*> ProjectGames;

	virtual void Initialize() override;
	virtual void Serialize(SerializedAsset& asset) override;
	virtual RenderPipeline& GetRenderer() override;
	virtual void CreateInitialScene() override;

private:
	Project ActiveProject;

	DX11Pipeline pipeline;
};