#pragma once

#include <Core.h>
#include <Math/Vector.h>

template<typename T = float, int Dimensions = 2> requires requires(T val, int dim) { std::is_arithmetic_v<T>; dim > 1; }
struct Bounds {
	constexpr Bounds() : position(), size() {}
	constexpr Bounds(Vector<T, Dimensions> size) : position(), size(size) {}
	constexpr Bounds(Vector<T, Dimensions> position, Vector<T, Dimensions> size) : position(position), size(size) {}
	constexpr Bounds(T x, T y, T width, T height) : position(x, y), size(width, height) {}
	constexpr Bounds(const Bounds& other) : position(other.position), size(other.size) {}

	Vector<T, Dimensions> position;
	Vector<T, Dimensions> size;

	constexpr bool Contains(const Vector<T, Dimensions>& Point) const {
		for (int i = 0; i < Dimensions; ++i) {
			if (Point.v[i] <= position.v[i]) return false;
			if (Point.v[i] >= (position.v[i] + size.v[i])) return false;
		}

		return true;
	}

	constexpr bool Overlap(const Bounds& other) const {
		// todo: fairly sure this can be optimised more than this
		bool valid = true;
		for (int i = 0; i < Dimensions; ++i) {
			bool result = false;
			T myPoint = position.v[i] + size.v[i];
			T otherPoint = other.position.v[i] + other.size.v[i];
			bool positive = size.v[i] >= 0;
			bool otherPositive = other.size.v[i] >= 0;

			result |= position[i] > other.position[i] && position[i] < otherPoint && otherPositive;
			result |= position[i] < other.position[i] && position[i] > otherPoint && !otherPositive;

			if (positive) {
				result |= position[i] < other.position[i] && myPoint > other.position[i];
				result |= position[i] < otherPoint && myPoint > otherPoint;
				
				result |= position[i] == other.position[i] && otherPositive;
				result |= position[i] == otherPoint && !otherPositive;
				result |= myPoint == other.position[i] && !otherPositive;
				result |= myPoint == otherPoint && otherPositive;
			} else {
				result |= position[i] > other.position[i] && myPoint < other.position[i];
				result |= position[i] > otherPoint && myPoint < otherPoint;

				result |= position[i] == other.position[i] && !otherPositive;
				result |= position[i] == otherPoint && otherPositive;
				result |= myPoint == other.position[i] && otherPositive;
				result |= myPoint == otherPoint && !otherPositive;
			}

			valid &= result;
		}
		
		return valid;
	}

	inline Bounds& operator=(const Bounds& other) {
		position = other.position;
		size = other.size;
		return *this;
	}

private:
	constexpr bool ExtractBit(unsigned long long val, int index) const {
		unsigned long long mask = 1 << index;
		return mask & val;
	}
};

typedef Bounds<int, 2> Bounds2Di;
typedef Bounds<int, 3> Bounds3Di;
typedef Bounds<float, 2> Bounds2D;
typedef Bounds<float, 3> Bounds3D;