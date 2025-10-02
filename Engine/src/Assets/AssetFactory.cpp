#include <Assets/AssetFactory.h>

std::map<std::string, std::function<SFObject* ()>>& AssetFactory::GetAssetTypeMap() {
	static std::map<std::string, std::function<SFObject* ()>> AssetTypeMap;
	return AssetTypeMap;
}