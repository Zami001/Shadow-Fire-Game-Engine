#include <Assets/AssetFactory.h>

std::map<const char*, std::function<SFObject* ()>>& AssetFactory::GetAssetTypeMap() {
	static std::map<const char*, std::function<SFObject* ()>> AssetTypeMap;
	return AssetTypeMap;
}