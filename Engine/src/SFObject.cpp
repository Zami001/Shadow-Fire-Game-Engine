#include <SFObject.h>
#include <typeinfo>
#include <Asserts.h>
#include <Game.h>

AssetType<SFObject> assetType;

SFObject::SFObject() : name(nullptr), UsingTick(false) {
	SetName("New Object");
}

SFObject::~SFObject() {
	delete[] name;
	DisableTick();
}

void SFObject::Serialize(SerializedAsset& asset) {
	asset.SetAssetType<SFObject>();

	std::string name = this->name;

	SERIALIZE_VALUE(asset, name);
	SERIALIZE_VALUE(asset, guid);
}

const char* SFObject::GetName() const {
	return name;
}

void SFObject::SetName(const char* NewName) {
	SF_ASSERT(NewName != nullptr, "Name cannot be set to nullptr")

	delete[] name;
	int size = strlen(NewName) + 1;
	name = new char[size];
	memcpy(name, NewName, size);
}

void SFObject::EnableTick() {
	if (UsingTick) return;

	GetGameInstance()->GetTickManager().RegisterTick(this, [this](float DeltaTime) { Tick(DeltaTime); });
	UsingTick = true;
}

void SFObject::DisableTick() {
	if (!UsingTick) return;

	GetGameInstance()->GetTickManager().UnregisterTick(this);
	UsingTick = false;
}
