#include <Math/Vector.h>
#include <iostream>

int Math_Vector(int argc, char** const arg) {
	Vector<int, 3> vec1;
	Vector<int, 3> vec2;
	Vector<int, 3> result;

	vec1 = { 1, 2, 3 };
	vec2 = { 1, 2, 3 };
	result = { 2, 4, 6 };
	
	if (vec1 + vec2 != result) {
		std::cout << "Add operation on vectors returns incorrect result";
		return 1;
	};

	result = { 0, 0, 0 };
	if (vec1 - vec2 != result) {
		std::cout << "Subtract operation on vectors returns incorrect result";
		return 1;
	};

	result = { 1, 4, 9 };
	if (vec1 * vec2 != result) {
		std::cout << "Multiply operation on vectors returns incorrect result";
		return 1;
	};

	result = { 1, 1, 1 };
	if (vec1 / vec2 != result) {
		std::cout << "Divide operation on vectors returns incorrect result";
		return 1;
	};

	return 0;
}