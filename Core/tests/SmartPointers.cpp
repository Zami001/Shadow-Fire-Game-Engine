#include <Containers/SFObjectContainer.h>
#include <iostream>

class baseType {};
class derivedType : public baseType {};

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

	return 0;
}