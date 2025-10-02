#pragma once

#include <Engine.h>
#include <map>
#include <functional>
#include <typeinfo>
#include <string>

class SFObject;

class ENGINE_API AssetFactory {
	template <typename T>
	friend class AssetType;

public:
	static std::map<std::string, std::function<SFObject* ()>>& GetAssetTypeMap();
private:

	template<typename T>
	static const char* GetClassName() {
		return typeid(T).name();
	}

public:
	static SFObject* CreateInstance(const std::string& ClassType) {
		if (ClassType == "Null") {
			return nullptr;
		}

		return GetAssetTypeMap().at(ClassType)();
	}
};