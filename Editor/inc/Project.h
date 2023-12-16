#pragma once

#include <Editor.h>
#include <SFObject.h>

#include <filesystem>
#include <string>

struct HINSTANCE__;

class EDITOR_API Project : public SFObject {
public:
	Project();
	~Project();

	virtual void Serialize(SerializedAsset& asset) override;

	void UnloadProject();

	void LoadProject();
	void LoadProject(std::filesystem::path project);

	void CreateProject();
	void CreateProject(std::filesystem::path project);
	
	std::filesystem::path GetProjectRoot() const;
	bool GetProjectLoaded() const;

	static bool HasFileAssociation();
	static bool SetupFileAssociations();

	void GenerateProjectFiles();

private:
	std::filesystem::path ProjectRoot;

	HINSTANCE__* DllHandle;
};