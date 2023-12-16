#pragma once

#include <Engine.h>
#include <utility>
#include <string>
#include <Assets/AssetFactory.h>
#include <iostream>
#include <Asserts.h>
#include <type_traits>

class SFObject;

template<typename T>
class AssetType {
private:
	template<typename U = T, typename std::enable_if_t<std::is_default_constructible<U>::value, bool> = true>
	void Init() {
		SF_ASSERT(AssetFactory::GetAssetTypeMap().count(AssetFactory::GetClassName<T>()) == 0, "Asset type already exists in class map, attempting to redefine asset name")
		AssetFactory::GetAssetTypeMap().insert({ AssetFactory::GetClassName<T>(), []() { return new T(); } });
	}

	template<typename U = T, typename std::enable_if_t<!std::is_default_constructible<U>::value, bool> = true>
	void Init() {
		SF_ASSERT(AssetFactory::GetAssetTypeMap().count(AssetFactory::GetClassName<T>()) == 0, "Asset type already exists in class map, attempting to redefine asset name")
		AssetFactory::GetAssetTypeMap().insert({ AssetFactory::GetClassName<T>(), []() { throw new std::exception("Attempted to instantiate an asset without a valid default constructor"); return nullptr; } });
	}

public:
	AssetType() {
		static_assert(std::is_base_of<SFObject, T>::value, "Asset types must derive from SFObject");
		Init();
	}

	static const char* GetAssetKey() {
		SF_ASSERT(AssetFactory::GetAssetTypeMap().count(AssetFactory::GetClassName<T>()) == 1, "Asset key requested for an asset type which has not been registered")
		return AssetFactory::GetClassName<T>();
	}
};