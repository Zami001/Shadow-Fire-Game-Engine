#include <Assets/SerializedAsset.h>
#include <string>
#include <Asserts.h>
#include <iostream>
#include <SFObject.h>

static const int majorversion = 1;
static const int minorversion = 0;

static bool RegisteringSubobject = false;

SerializedAsset::SerializedAsset(bool binary, bool read, bool editor) : BinaryMode(binary), ReadMode(read), EditorMode(editor) {}

SerializedAsset::~SerializedAsset() {}

void SerializedAsset::MarkSubobjectValues(bool Subobject) {
	RegisteringSubobject = Subobject;
}

void SerializedAsset::WriteToFile(const char* filename) {
	//std::ofstream file(filename, std::ios_base::trunc | (BinaryMode ? std::ios_base::binary : 0));
	//
	//if (file.is_open()) {
	//	if (BinaryMode) {
	//		file << '\0'; // binary meta marker
	//		WriteToBinaryFile(file);
	//	} else {
	//		file << "version: " << majorversion << "." << minorversion << '\n';
	//		WriteToTextFile(file, 0);
	//	}
	//
	//	file.close();
	//}
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

SerializedAsset::BinaryValue::BinaryValue(BinaryValue&& other) : Size(other.Size) {
	Value = other.Value;
	other.Value = nullptr;
}

SerializedAsset::BinaryValue::~BinaryValue() {
	delete[] Value;
}

void SerializedAsset::BinaryValue::operator=(const BinaryValue& other) {
	if (&other == this) return;

	delete[] Value;

	Size = other.Size;
	Value = new char[Size];
	memcpy(Value, other.Value, Size);
}

void SerializedAsset::SerializeSubobject(SFObject& object, const std::string& name, bool editoronly) {
	RegisteringSubobject = true;
	SerializedAsset subobject(BinaryMode, ReadMode, EditorMode);
	object.Serialize(subobject);

	if (BinaryMode) {
		// TODO: Binary version here
	} else {
		//TextValues.insert({ name, subobject.ToString() });
	}
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