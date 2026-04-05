#include <Containers/SFObjectContainer.h>
#include <iostream>

class baseType {};
class derivedType : public baseType {};

class SharableBase : public SharedFromThis<SharableBase> { public: virtual int Test() { return 0; } };
class SharableDerived : public SharableBase { public : virtual int Test() override { return 1; } };

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
	sharablePtr = nullptr;

	if (SharablePtr2->Test() != 1) {
		std::cout << "Function from SharedFromThis derived value failed to return correctly from SFSharedPtr";
		return 1;
	}

	SFWeakPtr<SharableBase> weakSharableValue = SharablePtr2;
	if (weakSharableValue->Test() != 1) {
		std::cout << "Function from SharedFromThis derived value failed to return correctly from SFWeakPtr";
		return 1;
	}

	SharablePtr2 = nullptr;
	if (weakSharableValue != nullptr) {
		std::cout << "Weak pointer created from SharedFromThis is not released while shared pointers are unset";
		return 1;
	}

	return 0;
}