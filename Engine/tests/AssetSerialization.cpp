#include <Assets/SerializedAsset.h>
#include <SFObject.h>
#include <iostream>

class TestChild;

class TestParent : public SFObject {
public:
	std::string string;
	std::vector<int> numbers;
	std::map<int, std::string> map;
	TestChild* subobject1;
	TestChild* subobject2;
	TestChild* subobject3;

	~TestParent() {
		delete subobject1;
		delete subobject2;
		delete subobject3;
	}

	virtual void Serialize(SerializedAsset& asset) override {
		SFObject::Serialize(asset);

		asset.SetAssetType<TestParent>();

		SERIALIZE_VALUE(asset, string);
		SERIALIZE_VALUE(asset, numbers);
		SERIALIZE_VALUE(asset, map);
		SERIALIZE_SUBOBJECT(asset, subobject1);
		SERIALIZE_SUBOBJECT(asset, subobject2);
		SERIALIZE_SUBOBJECT(asset, subobject3);
	}
};

class TestChild : public SFObject {
public:
	TestParent* Parent;
	TestChild* Sibling;

	virtual void Serialize(SerializedAsset& asset) override {
		SFObject::Serialize(asset);

		asset.SetAssetType<TestChild>();

		SERIALIZE_OBJECT_REF(asset, Parent);
		SERIALIZE_OBJECT_REF(asset, Sibling);
	}
};

AssetType<TestParent> parenttype;
AssetType<TestChild> childtype;

int AssetSerialization(int argc, char** const argv) {
	TestParent* TestObject = new TestParent();

	// setup test data for test object
	const char* string = "\\\\abcdefghijklmnopqrstuvwxyz:/\"\r\n ";
	const std::vector<int> numbers = { 1, 2, 3 };
	const std::map<int, std::string> map = { {1, "one"}, {2, "two"}, {3, "three"} };

	// assign test data to initial object
	TestObject->string = string;
	TestObject->numbers = numbers;
	TestObject->map = map;

	TestObject->subobject1 = new TestChild();
	TestObject->subobject1->Parent = TestObject;
	TestObject->subobject2 = new TestChild();
	TestObject->subobject2->Parent = TestObject;
	TestObject->subobject2->Sibling = TestObject->subobject1;
	
	// create serialized asset from test object
	SerializedAsset asset = SerializedAsset::CreateFromObject(TestObject);
	asset.WriteToFile("TestAsset.SFASSET");

	// remove test object
	delete TestObject;
	TestObject = nullptr;

	SerializedAsset writeAsset(false, false, true);
	writeAsset.ReadFromFile("TestAsset.SFASSET");

	TestObject = writeAsset.RecreateAsset<TestParent>();

	// check if the object has been recreated correctly
	if (TestObject == nullptr) {
		std::cout << "Test object has not been recreated";
		return 1;
	}

	if (TestObject->string != string) {
		std::cout << "String failed to be sanatised or unsantised correctly, before: " << string << " after: " << TestObject->string;
		return 1;
	}

	if (TestObject->subobject1 == nullptr) {
		std::cout << "Subobject has not been recreated";
		return 1;
	}

	if (TestObject->subobject1->Parent == nullptr) {
		std::cout << "Parent object reference has not been established";
		return 1;
	}

	if (TestObject->subobject1->Parent != TestObject) {
		std::cout << "Parent object reference has not been correctly established to the parent";
		return 1;
	}

	if (TestObject->subobject1->Sibling != nullptr) {
		std::cout << "Sibling object reference has been incorrectly set to an object";
		return 1;
	}

	if (TestObject->subobject2->Sibling != TestObject->subobject1) {
		std::cout << "Sibling object reference has not been correctly established to first subobject";
		return 1;
	}

	if (TestObject->subobject3 != nullptr) {
		std::cout << "Subobject that should be null has not been set to nullptr";
		return 1;
	}

	if (TestObject->numbers != numbers) {
		std::cout << "numbers in vector have not be recreated accurately";
		return 1;
	}

	if (TestObject->map != map) {
		std::cout << "map has not been recreated accurately";
		return 1;
	}

	return 0;
}