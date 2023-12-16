#pragma once

#include <Engine.h>
#include <StreamBuffers/StreamBuffer.h>
#include <map>
#include <vector>
#include <fstream>
#include <stack>
#include <Assets/AssetType.h>
#include <SFGUID.h>

#define SERIALIZE_VALUE(asset, var) asset##.##SerializeValue(var, #var)
#define SERIALIZE_SUBOBJECT(asset, var) asset##.##SerializeSubobject(var, #var)
#define SERIALIZE_SUBOBJECT_COLLECTION(asset, var) asset.MarkSubobjectValues(true); SERIALIZE_VALUE(asset, var)

#if SF_EDITOR
	#define SERIALIZE_EDITORONLY_VALUE(asset, var) asset##.##SerializeValue(var, #var, true)
	#define SERIALIZE_EDITORONLY_SUBOBJECT(asset, var) asset##.##SerializeSubobject(var, #var, true)
	#define SERIALIZE_EDITORONLY_SUBOBJECT_COLLECTION(asset, var) for (int i = 0; i < var##.size(); ++i) { asset.SerializeSubobject(Subcomponents[i], true); } SERIALIZE_VALUE(asset, var)
#else
	#define SERIALIZE_EDITORONLY_VALUE(asset, var)
	#define SERIALIZE_EDITORONLY_SUBOBJECT(asset, var) 
	#define SERIALIZE_EDITORONLY_SUBOBJECT_COLLECTION(asset, var) 
#endif

class SFObject;

class ENGINE_API SerializedAsset {
	friend SFObject;

private:
	template<typename T>
	struct CollectionType {
	private:
		template<typename U = T::iterator*>
		static int8_t test(U);

		template<typename U>
		static int16_t test(...);

	public:
		enum { value = sizeof(test<T>(0)) == sizeof(int8_t) };
	};

public:
	SerializedAsset(bool binary = false, bool read = true, bool editor = true);
	~SerializedAsset();
	
	void WriteToFile(const char* filename);

	template<typename T>
	void SerializeValue(T& value, const char* name, bool editoronly = false) {
		//if (!EditorMode && editoronly) {
		//	return;
		//}
		//
		//if (ReadMode) {
		//	if (BinaryMode) {
		//		ReadBinary<T>(value);
		//	} else {
		//		TextCollectionValues.insert({ name, {} });
		//		for (int i = 0; i < value.size(); ++i) {
		//			TextCollectionValues.at(name).push_back(ReadText<T>());
		//		}
		//	}
		//} else {
		//	if (BinaryMode) {
		//		WriteBinary<T>(value);
		//	} else {
		//		auto found = TextValues.find(name);
		//		if (found != TextValues.end()) {
		//			WriteText(value, found->second);
		//		}
		//	}
		//}
	}

	template<typename T>
	void SerializeValue(std::vector<T>& value, const char* name, bool editoronly = false) {
		//if (!EditorMode && editoronly) {
		//	return;
		//}
		//
		//if (ReadMode) {
		//	if (BinaryMode) {
		//		ReadBinary<T>(value);
		//	} else {
		//		TextValues.insert({ name, ReadText(value) });
		//	}
		//} else {
		//	if (BinaryMode) {
		//		WriteBinary<T>(value);
		//	} else {
		//		auto found = TextValues.find(name);
		//		if (found != TextValues.end()) {
		//			WriteText(value, found->second);
		//		}
		//	}
		//}
	}

	template<typename T>
	void SetAssetType() {
		AssetKey = AssetType<T>::GetAssetKey();
	}

	void MarkSubobjectValues(bool Subobject);
	void SerializeSubobject(SFObject& object, const std::string& name, bool editoronly = false);
	void SerializeObjectRef(SFObject& object, const std::string& name, bool editoronly = false);

private:
	struct BinaryValue {
		BinaryValue(void* value, uint32_t size);
		BinaryValue(std::istream& file);
		BinaryValue(const BinaryValue& other);
		BinaryValue(BinaryValue&& other);
		~BinaryValue();

		void operator=(const BinaryValue& other);

		char* Value;
		uint32_t Size;
	};

	std::string AssetKey;
	std::vector<BinaryValue> BinaryValues;
	bool BinaryMode;
	bool ReadMode;
	bool EditorMode;

	void WriteToBinaryFile(std::ofstream& file);
	//void WriteToTextFile(std::ofstream& file);
	void ReadFromBinaryFile(std::ifstream& file);
	//void ReadFromTextFile(std::ifstream& file);

	template<typename T>
	void ReadBinary(T& value) {
		BinaryValues.push_back({ &value, sizeof(T) });
	}

	template<typename T>
	void WriteBinary(T& value) {
		//value = *reinterpret_cast<T*>(BinaryValues[counter].Value);
	}

	template<>
	void WriteBinary<SFGUID>(SFGUID& value) {
		//value.Identifier = *reinterpret_cast<uint64_t*>(BinaryValues[counter].Value);
	}

	template<typename T, std::enable_if_t<!std::is_pointer<T>::value, bool> = true>
	std::string ReadText(std::vector<T>& value, std::string& str = std::string("")) {
		//str += std::string("{") + (CollectionType<T>::value ? " " : "\n");
		//for (int i = 0; i < value.size(); ++i) {
		//	str += ReadText(value[i]) + "\n";
		//}
		//str += "} ";
		//return str;
	}

	template<typename T, std::enable_if_t<std::is_pointer<T>::value, bool> = true>
	std::string ReadText(std::vector<T>& value, std::string& str = std::string("")) {
		//str += std::string("{") + (CollectionType<T>::value ? " " : "\n");
		//for (int i = 0; i < value.size(); ++i) {
		//	str += ReadText(*value[i]) + "\n";
		//}
		//str += "} ";
		//return str;
	}

	template<typename T, typename U>
	void WriteText(std::vector<T, U>& value, const std::string& text) {
		// TODO: rewrite this for single string collections
		//value.resize(collection.size());
		//for (int i = 0; i < collection.size(); ++i) {
		//	WriteText(value[i], collection[i]);
		//}
	}
};