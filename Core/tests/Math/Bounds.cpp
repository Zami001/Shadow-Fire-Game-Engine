#include <Math/Bounds.h>
#include <iostream>

#define OVERLAP_TRUE !(bounds1.Overlap(bounds2) && bounds2.Overlap(bounds1))
#define OVERLAP_FALSE bounds1.Overlap(bounds2) || bounds2.Overlap(bounds1)
#define PRINT_BOUNDS	std::string pos1 = bounds1.position.ToString(), pos2 = bounds2.position.ToString(), size1 = bounds1.size.ToString(), size2 = bounds2.size.ToString();\
						std::cout << "Bounds 1: position - " << pos1 << ", size - " << size1 << "\nBounds 2: position - " << pos2 << ", size - " << size2;

int Math_Bounds(int argc, char** const arg) {
	Bounds2Di bounds1, bounds2;

	bounds1.position = { 1, 1 };
	bounds1.size = { 2, 2 };
	bounds2 = bounds1;

	if (OVERLAP_TRUE) {
		std::cout << "Identical bounds not found to be overlapping\n";
		PRINT_BOUNDS;
		return 1;
	}

	bounds2.position = { 2, 2 };
	if (OVERLAP_TRUE) {
		std::cout << "overlapping bounds not found to be overlapping\n";
		PRINT_BOUNDS;
		return 1;
	}

	bounds2.position = { 1, 4 };
	if (OVERLAP_FALSE) {
		std::cout << "Non overlapping bounds found to be overlapping\n";
		PRINT_BOUNDS;
		return 1;
	}

	bounds2.position = { 1, 3 };
	if (OVERLAP_FALSE) {
		std::cout << "Touching but not overlapping bounds found to be overlapping\n";
		PRINT_BOUNDS;
		return 1;
	}

	bounds2.size = { 1, -1 };
	if (OVERLAP_TRUE) {
		std::cout << "Overlapping bounds utilizing negative size not found to be overlapping\n";
		PRINT_BOUNDS;
		return 1;
	}

	bounds2.position = { 0, 0 };
	bounds2.size = { 4, 4 };
	if (OVERLAP_TRUE) {
		std::cout << "Bounds contained entirely within another bounds not found to be overlapping\n";
		PRINT_BOUNDS;
		return 1;
	}

	bounds1.position = { -1, 2 };
	bounds1.size = { 6, 1 };
	if (OVERLAP_TRUE) {
		std::cout << "Overlapping bounds with only overlapping bodies not found to be overlapping\n";
		PRINT_BOUNDS;
		return 1;
	}

	bounds1.position = { 0, 0 };
	bounds1.size = { 2, 2 };
	if (OVERLAP_TRUE) {
		std::cout << "Overlapping bounds contained within the other bounds while touching the edge not found to be overlapping\n";
		PRINT_BOUNDS;
		return 1;
	}

	bounds1.position = { 0, 0 };
	bounds1.size = { 2, 5 };
	if (OVERLAP_TRUE) {
		std::cout << "Overlapping bounds contained within the other bounds while touching the edge not found to be overlapping\n";
		PRINT_BOUNDS;
		return 1;
	}

	return 0;
}