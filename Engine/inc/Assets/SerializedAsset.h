#pragma once

#include <Engine.h>
#include <StreamBuffers/StreamBuffer.h>
#include <map>
#include <vector>
#include <fstream>
#include <stack>
#include <Assets/AssetType.h>
#include <SFGUID.h>
#include <Containers/SFObjectContainer.h>

#define SERIALIZE_VALUE(asset, var) asset##.##SerializeValue(var, #var)
#define SERIALIZE_SUBOBJECT(asset, var) SERIALIZE_VALUE(asset, var) //asset##.##SerializeSubobject(var, #var)
#define SERIALIZE_SUBOBJECT_COLLECTION(asset, var) SERIALIZE_VALUE(asset, var)
#define SERIALIZE_OBJECT_REF(asset, var) asset##.##SerializeObjectRef(var, #var)

#if SF_EDITOR
	#define SERIALIZE_EDITORONLY_VALUE(asset, var) asset##.##SerializeValue(var, #var, true)
	#define SERIALIZE_EDITORONLY_SUBOBJECT(asset, var) asset##.##SerializeSubobject(var, #var, true)
	#define SERIALIZE_EDITORONLY_SUBOBJECT_COLLECTION(asset, var) SERIALIZE_EDITORONLY_VALUE(asset, var) //for (int i = 0; i < var##.size(); ++i) { asset.SerializeSubobject(Subcomponents[i], true); } SERIALIZE_VALUE(asset, var)
#else
	#define SERIALIZE_EDITORONLY_VALUE(asset, var)
	#define SERIALIZE_EDITORONLY_SUBOBJECT(asset, var) 
	#define SERIALIZE_EDITORONLY_SUBOBJECT_COLLECTION(asset, var) 
#endif

class SFObject;

class ENGINE_API SerializedAsset {
	friend SFObject;

public:
	SerializedAsset(bool binary = false, bool read = true, bool editor = true);
	SerializedAsset(SerializedAsset&& other) noexcept;
	~SerializedAsset();

private:
	SerializedAsset(SerializedAsset& Parent);

public:
	void WriteToFile(const char* filename);
	void ReadFromFile(const char* filename);

	template<typename T>
	void SerializeValue(T& value, const char* name, bool editoronly = false) {
		if (!EditorMode && editoronly) {
			return;
		}
		
		if (ReadMode) {
			if (BinaryMode) {
				//ReadBinary<T>(value);
			} else {
				StringValues.insert({ name, ReadText(value) });
			}
		} else {
			if (BinaryMode) {
				//WriteBinary<T>(value);
			} else {
				auto found = StringValues.find(name);
				if (found != StringValues.end()) {
					WriteText(value, found->second);
				}
			}
		}
	}

	template<typename T>
	void SetAssetType() {
		// don't write the asset key while writing to the object being serialized
		if (ReadMode) {
			AssetKey = AssetType<T>::GetAssetKey();
		}
	}

	void SerializeObjectRef(SFObject*& object, const std::string& name, bool editoronly = false);

	template<typename T>
	requires(std::is_base_of_v<SFObject, T>)
	void SerializeObjectRef(T*& object, const std::string& name, bool editoronly = false) {
		SerializeObjectRef(reinterpret_cast<SFObject*&>(object), name, editoronly);
	}

	SFObject* RecreateAsset();
	static SerializedAsset CreateFromObject(SFObject* object, bool binary = false, bool editor = true);

	template<typename T>
	requires(std::is_base_of_v<SFObject, T>)
	inline T* RecreateAsset() {
		SFObject* obj = RecreateAsset();
		T* returnVal = dynamic_cast<T*>(obj);

		if (returnVal == nullptr) {
			delete obj;
		}

		return returnVal;
	}

private:
	struct ENGINE_API BinaryValue {
		BinaryValue(void* value, uint32_t size);
		BinaryValue(std::istream& file);
		BinaryValue(const BinaryValue& other);
		BinaryValue(BinaryValue&& other) noexcept;
		~BinaryValue();

		void operator=(const BinaryValue& other);

		char* Value;
		uint32_t Size;
	};

	struct ENGINE_API AssetReferences {
	private:
		std::map<uint64_t, SFObject*> RecreatedObjects;
		std::map<uint64_t, std::vector<SFObject**>> AwaitingRawPtrs;
		std::map<uint64_t, std::vector<SFSharedPtr<SFObject>*>> AwaitingSharedPtrs;
		std::map<uint64_t, std::vector<SFSharedRef<SFObject>*>> AwaitingSharedRefs;
		std::map<uint64_t, std::vector<SFWeakPtr<SFObject>*>> AwaitingWeakPtrs;

	public:
		void Clear() {
			RecreatedObjects.clear();
			AwaitingRawPtrs.clear();
			AwaitingSharedPtrs.clear();
			AwaitingSharedRefs.clear();
			AwaitingWeakPtrs.clear();
		}

		void AddRecreatedObject(SFObject* NewObject);

		template<typename T, bool Threadsafe, bool Strong, bool AllowNull>
		requires(std::is_base_of_v<SFObject, T>)
		void AddPtr(SFObjectLink<T, Threadsafe, Strong, AllowNull>& ptr, uint64_t Identifier) {
			auto obj = RecreatedObjects.find(Identifier);
			if (obj != RecreatedObjects.end()) {
				ptr = reinterpret_cast<T*>(obj->second);
				return;
			}

			if constexpr (Strong && AllowNull) {
				AwaitingSharedPtrs[Identifier].push_back(reinterpret_cast<SFSharedPtr<SFObject>*>(&ptr));
			}

			if constexpr (!Strong && AllowNull) {
				AwaitingSharedRefs[Identifier].push_back(reinterpret_cast<SFSharedRef<SFObject>*>(&ptr));
			}

			if constexpr (Strong && !AllowNull) {
				AwaitingWeakPtrs[Identifier].push_back(reinterpret_cast<SFWeakPtr<SFObject>*>(&ptr));
			}
		}

		template<typename T>
		requires(std::is_base_of_v<SFObject, T>)
		void AddPtr(T*& ptr, uint64_t Identifier) {
			auto obj = RecreatedObjects.find(Identifier);
			if (obj != RecreatedObjects.end()) {
				ptr = reinterpret_cast<T*>(obj->second);
				return;
			}

			AwaitingRawPtrs[Identifier].push_back(reinterpret_cast<SFObject**>(&ptr));
		}
	};

	SerializedAsset* ParentAsset;
	AssetReferences* References;
	std::string AssetKey; // key required to reconstruct the serialized object
	std::vector<BinaryValue> BinaryValues;
	std::map<std::string, std::string> StringValues;
	bool BinaryMode;
	bool ReadMode; // Read mode of the serialized asset, true will read data from assets being serialized, false will write to assets being serialized
	bool EditorMode;
	int IndentLevel = 0;

	// output the string values of the serialized object to a string formatted for file writting
	std::string ToString();

	// extract a single kvp for a string. blank strings returned when none can be found
	std::pair<std::string, std::string> ExtractKeyValue(const std::string& string, int& outEndIndex, int offset = 0);
	void ReadTextToAsset(const std::string& string);

	void WriteToBinaryFile(std::ofstream& file);
	void ReadFromBinaryFile(std::ifstream& file);
	void ReadFromTextFile(std::ifstream& file);

	inline void ReplaceCharacter(std::string& str, char find, char replace) {
		int index = 0;
		do {
			index = str.find(find, index);
			if (index != -1) {
				str.replace(index, 1, 1, replace);
			}
		} while (index != -1);
	}

	static inline void ReplaceCharacter(std::string& str, const char* find, const char* replace) {
		int findSize = std::strlen(find);
		int replaceSize = std::strlen(replace);
		int sizeDiff = replaceSize - findSize;

		int index = 0;
		do {
			index = str.find(find, index);
			if (index != -1) {
				str.replace(index, findSize, replace);
				index += sizeDiff + 1;
			}
		} while (index != -1);
	}

	static inline void SanatiseString(std::string& str) {
		ReplaceCharacter(str, "\\", "\\\\");
		ReplaceCharacter(str, "\r", "\\r");
		ReplaceCharacter(str, "\n", "\\n");
		ReplaceCharacter(str, "\"", "\\\"");
		ReplaceCharacter(str, ":", "\\c");

		str = "\"" + str + "\"";
	}

	static inline void UnsanatiseString(std::string& str) {
		// remove surounding quotes
		if (str[0] == '\"') {
			str.erase(0, 1);
		}

		if (str[str.length() - 1] == '\"') {
			str.erase(str.length() - 1, 1);
		}

		std::map<char, char> Replacements = {
			{ 'r' , '\r'},
			{ 'n', '\n' },
			{ '"', '"' },
			{ 'c', ':' },
			{ '\\', '\\' }
		};

		int index = 0;
		do {
			index = str.find_first_of("\\", index);
			if (index == -1) {
				break;
			}

			str.erase(index, 1);
			auto replaceWith = Replacements.find(str[index]);
			if (replaceWith == Replacements.end()) {
				SF_LOG(LogSerializer, Error, "String contains escaped character that doesn't match an existing signature");
				return;
			}
			str[index] = replaceWith->second;
			index++;
		} while (index != -1);
	}

	inline std::string ReadText(const std::string& value) {
		std::string str = value;

		SanatiseString(str);

		return str;
	}

	inline void WriteText(std::string& value, const std::string& string) {
		std::string str = string;

		UnsanatiseString(str);

		value = str;
	}

	inline std::string ReadText(int value) {
		return std::to_string(value);
	}

	inline void WriteText(int& value, const std::string& string) {
		value = std::atoi(string.c_str());
	}

	inline std::string ReadText(unsigned int value) {
		return std::to_string(value);
	}

	inline void WriteText(unsigned int& value, const std::string& string) {
		value = std::stoul(string);
	}

	inline std::string ReadText(double value) {
		return std::to_string(value);
	}

	inline void WriteText(double& value, const std::string& string) {
		value = std::stod(string);
	}

	inline std::string ReadText(float value) {
		return std::to_string(value);
	}

	inline void WriteText(float& value, const std::string& string) {
		value = std::stof(string);
	}

	inline std::string ReadText(unsigned long long value) {
		return std::to_string(value);
	}

	inline void WriteText(unsigned long long& value, const std::string& string) {
		value = std::stoull(string);
	}

	inline std::string ReadText(SFGUID value) {
		return ReadText(value.Identifier);
	}

	inline void WriteText(SFGUID& value, const std::string& string) {
		WriteText(value.Identifier, string);
	}

	inline void AddIndent(std::string& str) const {
		std::string Indent;
		for (int i = 0; i < IndentLevel; ++i) {
			Indent += "\t";
		}
		str += Indent;
	}

	template<typename T>
	std::string ReadText(const std::vector<T>& value) {
		if (value.size() == 0) {
			return "[]";
		}

		std::string str = "[\r\n";
		IndentLevel++;

		for (int i = 0; i < value.size(); ++i) {
			AddIndent(str);
			str += ReadText(value[i]);
			str += "\r\n";
		}

		IndentLevel--;
		AddIndent(str);
		str += "]";

		return str;
	}

	// interpret string across multiple lines to limit to an object or collection string
	void ExtractObject(const std::string& string, int& outStartIndex, int& outEndIndex, int offset = 0);

	template<typename T>
	void WriteText(std::vector<T>& value, const std::string& string) {
		value.clear();

		int startIndex = string.find_first_not_of("\r\n\t ");
		if (startIndex == -1) {
			throw new std::exception("Could not find valid characters");
		}

		if (string[startIndex] != '[') {
			throw new std::exception("Object is not a valid collection");
		}
		
		startIndex = string.find_first_of("\r\n", startIndex);
		do {
			int finishIndex;
			ExtractObject(string, startIndex, finishIndex, startIndex);
			std::string substr = string.substr(startIndex, finishIndex - startIndex + 1);
			T newValue;
			WriteText(newValue, substr);
			value.push_back(newValue);

			startIndex = string.find_first_not_of("]\r\n\t ", startIndex + 1);
			if (startIndex == -1) {
				break;
			}
		} while (startIndex != -1);
	}

	template<typename T, typename U>
	std::string ReadText(const std::map<T, U>& value) {
		if (value.size() == 0) {
			return "[]";
		}

		std::string str = "[\r\n";
		IndentLevel++;

		for (auto& [key, val] : value) {
			AddIndent(str);
			str += ReadText(key);
			str += " : ";
			str += ReadText(val);
			str += "\r\n";
		}

		IndentLevel--;
		AddIndent(str);
		str += "]";

		return str;
	}

	template<typename T, typename U>
	void WriteText(std::map<T, U>& value, const std::string& string) {
		value.clear();

		int startIndex = string.find_first_not_of("\r\n\t ");
		if (startIndex == -1) {
			throw new std::exception("Could not find valid characters");
		}

		if (string[startIndex] != '[') {
			throw new std::exception("Object is not a valid collection");
		}

		startIndex = string.find_first_of("\r\n", startIndex);
		do {
			int finishIndex;
			std::pair<std::string, std::string> stringpair = ExtractKeyValue(string, finishIndex, startIndex);
			std::pair<T, U> newPair;
			WriteText(newPair.first, stringpair.first);
			WriteText(newPair.second, stringpair.second);
			value.insert(newPair);

			startIndex = finishIndex;
			startIndex = string.find_first_not_of("]\r\n\t ", startIndex + 1);
			if (startIndex == -1) {
				break;
			}
		} while (startIndex != -1);
	}

	std::string ReadText(SFObject* value);

	template<typename T>
	requires(std::is_base_of_v<SFObject, T>)
	void WriteText(T*& value, const std::string& string) {
		if (string == "Null") {
			value = nullptr;
			return;
		}

		SerializedAsset Subasset(*this);
		Subasset.ReadTextToAsset(string);

		value = dynamic_cast<T*>(AssetFactory::CreateInstance(Subasset.AssetKey));
		value->Serialize(Subasset);
		References->AddRecreatedObject(value);
	}
	
	template<typename T, bool Threadsafe, bool StrongLink, bool AllowNull>
	std::enable_if_t<std::is_base_of_v<SFObject, T>, std::string> ReadText(SFObjectLink<T, Threadsafe, StrongLink, AllowNull> value) {
		return ReadText(value.Get());
	}

	template<typename T, bool Threadsafe, bool Strong, bool AllowNull>
	requires(std::is_base_of_v<SFObject, T>)
	inline void WriteText(SFObjectLink<T, Threadsafe, Strong, AllowNull>& value, const std::string& string) {
		SerializedAsset Subasset(*this);
		Subasset.ReadTextToAsset(string);

		value = dynamic_cast<T*>(AssetFactory::CreateInstance(Subasset.AssetKey));
		value->Serialize(Subasset);
		References->AddRecreatedObject(value);
	}
};