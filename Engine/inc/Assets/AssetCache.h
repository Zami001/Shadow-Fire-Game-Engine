#pragma once

#include <Engine.h>
#include <SFObject.h>
#include <map>

class ENGINE_API AssetCache : public SFObject {
public:
	virtual void Serialize(SerializedAsset& asset) override;

	std::map<SFGUID, const char*> AssetDirectory;
};