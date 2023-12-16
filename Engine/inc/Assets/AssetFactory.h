#pragma once

#include <Engine.h>
#include <map>
#include <functional>
#include <typeinfo>

class SFObject;

class ENGINE_API AssetFactory {
	template <typename T>
	friend class AssetType;

public:
	static std::map<const char*, std::function<SFObject* ()>>& GetAssetTypeMap();
private:

	template<typename T>
	static const char* GetClassName() {
		return typeid(T).name();
	}

public:
	SFObject* CreateInstance(const char* ClassType) {
		return GetAssetTypeMap().at(ClassType)();
	}
};