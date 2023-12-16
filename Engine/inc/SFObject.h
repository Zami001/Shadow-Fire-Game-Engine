#pragma once

#include <Engine.h>
#include <string>
#include <SFGUID.h>
#include <Assets/SerializedAsset.h>
#include <Assets/AssetType.h>
#include <MinimalCore.h>

class SerializedAsset;
class Game;
class GameObject;
class Component;
class Scene;

template<typename T>
concept PlainObject = requires(T val) {
	requires std::is_base_of_v<SFObject, T>;
	requires !std::is_base_of_v<GameObject, T>;
	requires !std::is_base_of_v<Component, T>;
};

class ENGINE_API SFObject {
	friend Game;
	friend GameObject;
	friend Component;
	friend Scene;

protected:
	SFObject();
	virtual ~SFObject();

public:
	/// <summary>
	/// <para>Serialize an object into an asset format.</para>
	/// <para>In order to recreate the object without an existing object, <see cref="SerializedObject::SetAssetType">SetAssetType</see>
	///  must be called in the overridden function</para>
	/// </summary>
	/// <param name="asset">Asset to serialize into or out of, as determined by the read/write mode of the asset</param>
	virtual void Serialize(SerializedAsset& asset);

	const char* GetName() const;
	void SetName(const char* name);

	inline const SFGUID& GetGUID() const { return guid; }

	inline Game* GetGameInstance() const {
		return GameInstance;
	}

	template<typename T>
	inline T* GetGameInstance() const {
		return dynamic_cast<T>(GameInstance);
	}

	template<PlainObject T, typename... Args>
	inline SFSharedRef<T> NewObject(Args... args) {
		SFSharedRef<T> obj = ConstructObject<T, Args...>(args...);
		obj->FinishConstruction();
		return obj;
	}

	inline void FinishConstruction() {
		Initialize();
	}

protected:
	virtual void Tick(float DeltaTime) {}
	void EnableTick();
	void DisableTick();

	virtual void Initialize() {};

private:
	SFGUID guid;
	char* name;

	Game* GameInstance;
	bool UsingTick;

	template<typename T, typename... Args>
	inline SFSharedRef<T> ConstructObject(Args... args) {
		SFSharedRef<T> obj = new T(args...);
		obj->GameInstance = GameInstance;
		return obj;
	}
};