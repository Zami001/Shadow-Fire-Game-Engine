#include <Project.h>
#include <Application.h>
#include <Asserts.h>

#include <Platform/Windows/PreWindowsInclude.h>
#include <windows.h>
#include <Platform/Windows/PostWindowsInclude.h>

Project::Project() : DllHandle(nullptr) {}

Project::~Project() {
	if (DllHandle) {
		//FreeLibrary((HMODULE)DllHandle);
	}
}

void Project::Serialize(SerializedAsset& asset) {
	SFObject::Serialize(asset);

	asset.SetAssetType<Project>();
}

void Project::UnloadProject() {

}

void Project::LoadProject() {
	OPENFILENAME ofn;       // common dialog box structure
	char szFile[260];       // buffer for file name
	szFile[0] = '\0';
	
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "Shadow Fire Project\0*.SFPROJECT\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&ofn)) {
		LoadProject(szFile);
	} else {
		std::cout << "Failed to open project file" << '\n';
		//exit(0);
	}
}

#include <Assets/AssetFactory.h>
typedef void (__stdcall* importedfunc)();

void Project::LoadProject(std::filesystem::path project) {
	SF_ASSERT(std::filesystem::exists(project), "File path given for project load doesn't exist")

	ProjectRoot = project.parent_path();

	SetName(project.filename().replace_extension().string().c_str());
	std::cout << GetName() << '\n';
	
	for (auto it : std::filesystem::directory_iterator(project.parent_path())) {
		std::cout << it.path().string() << '\n';
	}

	GenerateProjectFiles();

	std::cout  << "Asset type map size: " << AssetFactory::GetAssetTypeMap().size() << "\n";

	std::filesystem::path dllpath = ProjectRoot;
	dllpath /= std::string("Binaries\\") + GetName() + "-Editor.dll";
	if (std::filesystem::exists(dllpath)) {
		DllHandle = LoadLibraryA(dllpath.string().c_str());
		if (DllHandle != nullptr) {
			std::cout << "Dll has been loaded\n";
			importedfunc func = (importedfunc)GetProcAddress(DllHandle, "TestFunc");
			if (func != NULL) {
				func();
			} else {
				std::cout << "could not load dll function\n";
			}
		} else {
			std::cout << "Failed to load dll\n";
		}
	}

	std::cout << "Asset type map size: " << AssetFactory::GetAssetTypeMap().size() << "\n";
}

std::filesystem::path Project::GetProjectRoot() const {
	return ProjectRoot;
}

bool Project::GetProjectLoaded() const {
	return !ProjectRoot.empty();
}

void Project::CreateProject() {
	OPENFILENAME ofn;       // common dialog box structure
	char szFile[260];       // buffer for file name
	szFile[0] = '\0';

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "Shadow Fire Project\0*.SFPROJECT";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;

	if (GetSaveFileName(&ofn)) {
		std::filesystem::path projectPath(szFile);
		projectPath.replace_extension(".SFPROJECT");

		CreateProject(szFile);
	} else {
		std::cout << "Failed to open project file" << '\n';
		//exit(0);
	}
}

void Project::CreateProject(std::filesystem::path project) {
	std::cout << project.extension().string();
	SF_ASSERT(!std::filesystem::exists(project), "Cannot create a project where one already exists")
	SF_ASSERT(project.extension().string() == ".SFPROJECT", "Create project must be called with a file extension of .SFPROJECT")
	SF_ASSERT(project.is_absolute(), "Create project must be called using an absolute path")
	
	if (!std::filesystem::exists(project.parent_path())) {
		if (!std::filesystem::create_directory(project.parent_path())) {
			// TODO: This shouldn't be an assert
			SF_ASSERT(false, "Failed to create directory for project")
		}
	} else {
		bool empty = true;
		for (auto it : std::filesystem::directory_iterator(project.parent_path())) {
			empty = false;
			break;
		}

		SF_ASSERT(empty, "Cannot create project in a non empty folder")
	}



	// Execute cmake commands to build project structure
	STARTUPINFOA si;
	PROCESS_INFORMATION pi;

	// set the size of the structures
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	char command[1024];
	const char* commandstr = "cmd /c mkdir \"C:\\Shadow Fire Game Engine\\TestFolder\\\""; // 
	memcpy(&command[0], commandstr, strlen(commandstr) + 1);

	// execute cmake batch file
	if (CreateProcessA(NULL, command, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
		std::cout << "This is a test" << '\n';
	}
	// Close process and thread handles. 
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);


}


bool SetRegistryValue(const char* Key, const char* value, const char* valuename = nullptr) {
	HKEY key;
	int result = RegCreateKeyEx(HKEY_CURRENT_USER, Key, 0, NULL, 0, KEY_WRITE, NULL, &key, NULL);

	if (result == ERROR_SUCCESS) {
		RegSetKeyValue(key, NULL, valuename, REG_SZ, value, strlen(value) + 1);
		RegCloseKey(key);
		return true;
	} else {
		return false;
	}
}

bool Project::HasFileAssociation() {
	HKEY key;
	if (RegOpenKeyEx(HKEY_CURRENT_USER, "SOFTWARE\\Classes\\ShadowFire.ProjectFile", 0, KEY_READ, &key) == ERROR_SUCCESS) {
		RegCloseKey(key);
		return true;
	}

	return false;
}

bool Project::SetupFileAssociations() {
	bool result = true;
	std::string appPath = std::string("\"") + Application::GetApplicationPath().string() + "\"";
	result &= SetRegistryValue("SOFTWARE\\Classes\\.SFPROJECT", "ShadowFire.ProjectFile");
	result &= SetRegistryValue("SOFTWARE\\Classes\\ShadowFire.ProjectFile", "Shadow fire project file");
	result &= SetRegistryValue("SOFTWARE\\Classes\\ShadowFire.ProjectFile\\shell\\open", "Open");
	result &= SetRegistryValue("SOFTWARE\\Classes\\ShadowFire.ProjectFile\\shell\\open\\command", (appPath + " \"%1\"").c_str());
	result &= SetRegistryValue("SOFTWARE\\Classes\\ShadowFire.ProjectFile\\shell\\generate", "Generate project files...");
	result &= SetRegistryValue("SOFTWARE\\Classes\\ShadowFire.ProjectFile\\shell\\generate", appPath.c_str(), "Icon");
	result &= SetRegistryValue("SOFTWARE\\Classes\\ShadowFire.ProjectFile\\shell\\generate\\command", appPath.c_str());

	if (result) {
		std::cout << "Successfuly configured file associations\n";
	} else {
		std::cout << "Failed to configure file associations\n";
	}

	return result;
}

void FindFiles(std::string& files, const std::filesystem::path& folder, char seperator = ' ') {
	for (auto it : std::filesystem::directory_iterator(folder)) {
		if (it.is_directory()) {
			FindFiles(files, it, seperator);
		} else if (it.is_regular_file()) {
			std::filesystem::path file = it;
			files += seperator + file.string();
		}
	}
}

#include <fstream>

void StringReplacement(const std::map<std::string, std::string*>& lookup, std::string& line) {
	if (line.size() < 5) {
		return;
	}

	size_t start = line.find("##");

	if (start == ~0) {
		return;
	}

	size_t end = line.find("##", start + 2);

	if (end == ~0) {
		return;
	}

	std::string key = line.substr(start + 2, end - start - 2);

	auto val = lookup.find(key);
	if (val != lookup.end()) {
		line.replace(start, end - start + 2, *val->second);
		StringReplacement(lookup, line);
	}
}

std::string& SwapChar(std::string& str, char OldChar, char NewChar) {
	int found;

	do {
		found = str.find(OldChar);

		if (found != ~0) {
			str[found] = NewChar;
			continue;
		}

		break;
	} while (true);
	
	return str;
}

void Project::GenerateProjectFiles() {
	std::string incfiles = "";
	std::string srcfiles = "";

	std::filesystem::path IncPath = ProjectRoot;
	IncPath /= "Scripts\\inc";
	std::filesystem::path SrcPath = ProjectRoot;
	SrcPath /= "Scripts\\src";

	for (const auto& it : std::filesystem::recursive_directory_iterator(IncPath)) {
		if (it.is_regular_file()) {
			auto str = it.path().string();
			incfiles += "\"" + SwapChar(str, '\\', '/') + "\" ";
		}
	}

	for (const auto& it : std::filesystem::recursive_directory_iterator(SrcPath)) {
		if (it.is_regular_file()) {
			auto str = it.path().string();
			srcfiles += "\"" + SwapChar(str, '\\', '/') + "\" ";
		}
	}

	std::filesystem::path from = Application::GetApplicationPath();
	from /= "Templates\\Project.cmake";

	std::filesystem::path dst = ProjectRoot;
	dst /= "Intermediate\\CMakeLists.txt";

	std::ifstream input(from);
	std::ofstream output(dst);

	if (!input.is_open() || !output.is_open()) {
		std::cout << "Error opening cmake project files\n";
		return;
	}

	// insert auto generation message
	output << "# This file is auto generated. Do not edit directly as changes will be overwriten\n\n";

	std::string projectname = std::string("\"") + GetName() + "\"";
	std::map<std::string, std::string*> ReplacementMap;
	ReplacementMap.insert({ "SRC_FILES", &srcfiles });
	ReplacementMap.insert({ "HEADER_FILES", &incfiles });
	ReplacementMap.insert({ "PROJECT_NAME", &projectname });

	char buffer[1024];
	std::string line;
	int readoffset = 0;
	int writeoffset = 0;
	while (!input.eof()) {
		memset(&buffer[writeoffset], 0, 1024 - writeoffset);
		input.read(&buffer[writeoffset], 1024 - writeoffset);

		char* found = nullptr;
		found = strchr(&buffer[writeoffset], '\n');
		while(found) {
			line.assign(&buffer[readoffset], found);

			StringReplacement(ReplacementMap, line);

			output << line << "\n";

			writeoffset = ((int)found) - ((int)buffer) + 1;
			readoffset = writeoffset;
			found = strchr(&buffer[writeoffset], '\n');
		};

		if (!input.eof()) {
			memcpy(buffer, &buffer[writeoffset], 1024 - writeoffset);
			writeoffset = 1024 - writeoffset;
			readoffset = 0;
		}
	}

	// finish the last line
	line.assign(&buffer[readoffset]);
	StringReplacement(ReplacementMap, line);
	output << line;
}