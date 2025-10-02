#include <Assets/SerializedAsset.h>
#include <string>
#include <Asserts.h>
#include <iostream>
#include <SFObject.h>

static constexpr int majorversion = 1;
static constexpr int minorversion = 0;

SerializedAsset::SerializedAsset(bool binary, bool read, bool editor) : BinaryMode(binary), ReadMode(read), EditorMode(editor), ParentAsset(nullptr) {
	References = new AssetReferences();
}

SerializedAsset::SerializedAsset(SerializedAsset&& other) noexcept : BinaryMode(other.BinaryMode), ReadMode(other.ReadMode), EditorMode(other.EditorMode), ParentAsset(other.ParentAsset), References(other.References), IndentLevel(other.IndentLevel) {
	StringValues = std::move(other.StringValues);
	BinaryValues = std::move(other.BinaryValues);
	AssetKey = std::move(other.AssetKey);
	other.References = nullptr;
}

SerializedAsset::SerializedAsset(SerializedAsset& Parent) : BinaryMode(Parent.BinaryMode), ReadMode(Parent.ReadMode), EditorMode(Parent.EditorMode), ParentAsset(&Parent), References(Parent.References) {}

SerializedAsset::~SerializedAsset() {
	if (ParentAsset == nullptr) {
		delete References;
	}
}

void SerializedAsset::WriteToFile(const char* filename) {
	std::ofstream file(filename, std::ios_base::trunc | (BinaryMode ? std::ios_base::binary : 0));
	
	if (file.is_open()) {
		if (BinaryMode) {
			file << '\0'; // binary meta marker
			WriteToBinaryFile(file);
		} else {
			file << "version: " << majorversion << "." << minorversion << '\n';
			file << ToString();
		}
	
		file.close();
	} else {
		SF_LOG(LogSerializer, Error, "Failed to open file for writing - %s", filename)
	}
}

void SerializedAsset::ReadFromFile(const char* filename) {
	std::ifstream file(filename, 0 | (BinaryMode ? std::ios_base::binary : 0));

	if (file.is_open()) {
		if (BinaryMode) {
			ReadFromBinaryFile(file);
		} else {
			std::string versionLine;
			std::getline(file, versionLine);
			ReadFromTextFile(file);
		}

		file.close();
	} else {
		SF_LOG(LogSerializer, Error, "Failed to open file for reading - %s", filename)
	}
}

SerializedAsset::BinaryValue::BinaryValue(void* value, uint32_t size) : Size(size) {
	Value = new char[size];
	memcpy(Value, value, size);
}

SerializedAsset::BinaryValue::BinaryValue(std::istream& file) {
	file.read(reinterpret_cast<char*>(&Size), sizeof(Size));
	Value = new char[Size];
	file.read(Value, Size);
}

SerializedAsset::BinaryValue::BinaryValue(const BinaryValue& other) : Size(other.Size) {
	Value = new char[Size];
	memcpy(Value, other.Value, Size);
}

SerializedAsset::BinaryValue::BinaryValue(BinaryValue&& other) noexcept : Size(other.Size) {
	Value = other.Value;
	other.Value = nullptr;
}

SerializedAsset::BinaryValue::~BinaryValue() {
	delete[] Value;
}

void SerializedAsset::AssetReferences::AddRecreatedObject(SFObject* NewObject) {
	uint64_t identifier = NewObject->GetGUID().Identifier;
	RecreatedObjects.insert({ identifier, NewObject });

	auto RawPtrs = AwaitingRawPtrs.find(identifier);
	if (RawPtrs != AwaitingRawPtrs.end()) {
		for (auto& ptr : RawPtrs->second) {
			*ptr = NewObject;
		}

		AwaitingRawPtrs.erase(RawPtrs);
	}

	auto SharedPtrs = AwaitingSharedPtrs.find(identifier);
	if (SharedPtrs != AwaitingSharedPtrs.end()) {
		for (auto& ptr : SharedPtrs->second) {
			*ptr = NewObject;
		}

		AwaitingSharedPtrs.erase(SharedPtrs);
	}

	auto SharedRefs = AwaitingSharedRefs.find(identifier);
	if (SharedRefs != AwaitingSharedRefs.end()) {
		for (auto& ptr : SharedPtrs->second) {
			*ptr = NewObject;
		}

		AwaitingSharedRefs.erase(SharedRefs);
	}

	auto WeakPtrs = AwaitingWeakPtrs.find(identifier);
	if (WeakPtrs != AwaitingWeakPtrs.end()) {
		for (auto& ptr : SharedPtrs->second) {
			*ptr = NewObject;
		}

		AwaitingWeakPtrs.erase(WeakPtrs);
	}
}

void SerializedAsset::BinaryValue::operator=(const BinaryValue& other) {
	if (&other == this) return;

	delete[] Value;

	Size = other.Size;
	Value = new char[Size];
	memcpy(Value, other.Value, Size);
}

void SerializedAsset::SerializeObjectRef(SFObject*& object, const std::string& name, bool editoronly) {
	if (!EditorMode && editoronly) {
		return;
	}

	if (BinaryMode) {

	} else {
		if (ReadMode) {
			std::string reference;
			if (object == nullptr) {
				reference = "Null";
			} else {
				reference = ReadText(object->GetGUID());
			}

			StringValues.insert({ name, reference });
		} else {
			auto found = StringValues.find(name);
			if (found != StringValues.end()) {
				if (found->second == "Null") {
					object = nullptr;
				} else {
					uint64_t Identifier;
					WriteText(Identifier, found->second);
					References->AddPtr(object, Identifier);
				}
			}
		}
	}
}

SFObject* SerializedAsset::RecreateAsset() {
	bool oldReadMode = ReadMode;
	ReadMode = false;

	SFObject* obj = AssetFactory::CreateInstance(AssetKey);
	obj->Serialize(*this);

	ReadMode = oldReadMode;

	References->AddRecreatedObject(obj);
	References->Clear();
	return obj;
}

SerializedAsset SerializedAsset::CreateFromObject(SFObject* object, bool binary, bool editor) {
	if (!object) {
		return SerializedAsset();
	}

	SerializedAsset asset(binary, true, editor);
	object->Serialize(asset);
	return asset;
}

std::string SerializedAsset::ToString() {
	std::string str = AssetKey + "\r\n";

	for (auto& [key, value] : StringValues) {
		AddIndent(str);
		str += key;
		str += " : ";
		str += value;
		str += "\r\n";
	}

	return str;
}

std::pair<std::string, std::string> SerializedAsset::ExtractKeyValue(const std::string& string, int& outEndIndex, int offset) {
	int objectStart = string.find_first_not_of("\t\r\n ", offset);
	int seperator = string.find_first_of(":", objectStart);

	if (seperator == -1) {
		return { "", "" };
	}

	int objectEnd = string.find_last_not_of(":\r\n\t ", seperator);

	std::string key = string.substr(objectStart, objectEnd - objectStart + 1);

	ExtractObject(string, objectStart, objectEnd, seperator + 1);
	std::string value = string.substr(objectStart, objectEnd - objectStart);
	outEndIndex = objectEnd;
	return { key, value };
}

void SerializedAsset::ReadTextToAsset(const std::string& string) {
	int objectStart = 0;
	int objectEnd = 0;
	std::string key;
	std::string value;

	// read first line as asset key
	objectStart = string.find_first_not_of("{\t\r\n ", 0);
	objectEnd = string.find_first_of("\r\n", objectStart);
	int AssetKeyEnd = string.find_last_not_of("\t\r\n ", objectEnd);
	AssetKey = string.substr(objectStart, AssetKeyEnd - objectStart + 1);
	objectStart = AssetKeyEnd + 1;

	// read remaining lines as object data
	do {
		std::pair<std::string, std::string> kvp = ExtractKeyValue(string, objectStart, objectStart);

		if (kvp.first == "") {
			break;
		}

		StringValues.insert(std::move(kvp));
	} while (true);
}

void SerializedAsset::WriteToBinaryFile(std::ofstream& file) {
	file << AssetKey;
	
	char sizebuffer[sizeof(size_t)];
	for (int i = 0; BinaryValues.size(); ++i) {
		memcpy(sizebuffer, &BinaryValues[i].Size, 4);
		file.write(sizebuffer, 4);
		file.write(BinaryValues[i].Value, BinaryValues[i].Size);
	}
}

void SerializedAsset::ReadFromBinaryFile(std::ifstream& file) {
	BinaryValues.clear();

	while (!file.eof()) {
		BinaryValues.push_back(file);
	}
}

void SerializedAsset::ReadFromTextFile(std::ifstream& file) {
	StringValues.clear();
	
	long long currentPos = file.tellg();
	file.seekg(0, std::ios::end);
	long long filesize = file.tellg() - currentPos;
	std::string string(filesize, '\0');
	file.seekg(currentPos);
	file.read(&string[0], filesize);
	
	ReadTextToAsset(string);
}

void SerializedAsset::ExtractObject(const std::string& string, int& outStartIndex, int& outEndIndex, int offset) {
	outStartIndex = string.find_first_not_of("\r\n\t ", offset);
	int objectCount = 0;
	int collectionCount = 0;

	if (outStartIndex == -1) {
		throw new std::exception("failed to find object data in string");
	}

	int searchIndex = outStartIndex;
	do {
		searchIndex = string.find_first_not_of("\r\n\t ", searchIndex);
		if (searchIndex == -1) {
			throw new std::exception("malformed object string, not all objects closed");
		}

		switch (string[searchIndex]) {
			case '[':
				collectionCount++;
				break;

			case '{':
				objectCount++;
				break;

			case ']':
				collectionCount--;
				break;

			case '}':
				objectCount--;
				break;

			default: // should encompass all single line values
				// skip to end of line upon finding non object marker
				if (collectionCount == 0 && objectCount == 0) {
					int lineEnd = string.find_first_of("\r\n", searchIndex);

					if (lineEnd == -1) {
						lineEnd = string.size() - 1;
					}

					// remove trailing white space and new lines
					lineEnd = string.find_last_not_of("\r\n\t ", lineEnd);

					outStartIndex = searchIndex;
					outEndIndex = lineEnd + 1;
					return;
				} else {
					searchIndex = string.find_first_of(":\r\n", searchIndex);
				}
				break;
		}

		searchIndex++;
	} while (collectionCount != 0 || objectCount != 0);

	outEndIndex = searchIndex;
}

std::string SerializedAsset::ReadText(SFObject* value) {
	if (value == nullptr) {
		return "Null";
	}

	SerializedAsset subobject(BinaryMode, ReadMode, EditorMode);
	subobject.IndentLevel = IndentLevel + 1;

	value->Serialize(subobject);
	std::string str;

	str += "{\r";
	subobject.AddIndent(str);
	str += subobject.ToString();
	AddIndent(str);
	str += "}";

	return str;
}