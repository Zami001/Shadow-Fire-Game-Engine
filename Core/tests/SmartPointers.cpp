#include <Containers/SFObjectContainer.h>
#include <iostream>

class baseType {};
class derivedType : public baseType {};

class SharableBase : public SharedFromThis<SharableBase> {};
class SharableDerived : public SharableBase { int Test() { return 0; } };

static_assert(Sharable<SharableBase, false>, "SharableBase is not flagged as sharable");
static_assert(Sharable<SharableDerived, false>, "SharableDerived is not flagged as sharable");

int SmartPointers(int argc, char** const args) {
	derivedType* value = new derivedType();
	SFSharedPtr<derivedType> sharedValue;
	SFWeakPtr<derivedType> weakValue;
	SFSharedPtr<baseType> sharedBaseValue;

	sharedValue = value;
	weakValue = sharedValue;
	sharedBaseValue = sharedValue;

	sharedValue = nullptr;

	if (weakValue == nullptr) {
		std::cout << "Weak pointer is released while shared pointers remain";
		return 1;
	}

	sharedBaseValue = nullptr;

	if (weakValue != nullptr) {
		std::cout << "Weak pointer is not released while shared pointers are unset";
		return 1;
	}

	SharableDerived* sharableValue = new SharableDerived();
	SFSharedPtr<SharableBase> sharablePtr = sharableValue;

	SFSharedPtr<SharableDerived> SharablePtr2 = sharableValue;
	

	return 0;
}