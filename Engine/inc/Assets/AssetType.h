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
	static_assert(std::is_base_of_v<SFObject, T>, "Asset types must be derived from SFObject");

	void Init() {
		SF_ASSERT(AssetFactory::GetAssetTypeMap().count(AssetFactory::GetClassName<T>()) == 0, "Asset type already exists in class map, attempting to redefine asset name");
		if constexpr (std::is_default_constructible<T>::value) {
			AssetFactory::GetAssetTypeMap().insert({ AssetFactory::GetClassName<T>(), []() { return new T(); } });
		} else {
			AssetFactory::GetAssetTypeMap().insert({ AssetFactory::GetClassName<T>(), []() { throw new std::exception("Attempted to instantiate an asset type without a valid default constructor"); return nullptr; } });
		}
	}

public:
	AssetType() {
		Init();
	}

	static const char* GetAssetKey() {
		SF_ASSERT(AssetFactory::GetAssetTypeMap().count(AssetFactory::GetClassName<T>()) == 1, "Asset key requested for an asset type which has not been registered")
		return AssetFactory::GetClassName<T>();
	}
};