#pragma once

#include <Core.h>
#include <Math/Vector.h>

template<typename T = float, int Dimensions = 2> requires requires(T val, int dim) { std::is_arithmetic_v<T>; dim > 1; }
struct Bounds {
	constexpr Bounds() : position(), size() {}
	constexpr Bounds(Vector<T, Dimensions> size) : position(), size(size) {}
	constexpr Bounds(Vector<T, Dimensions> position, Vector<T, Dimensions> size) : position(position), size(size) {}
	constexpr Bounds(T x, T y, T width, T height) : position(x, y), size(width, height) {}

	Vector<T, Dimensions> position;
	Vector<T, Dimensions> size;

	inline bool Contains(Vector<T, Dimensions> Point) const {
		for (int i = 0; i < Dimensions; ++i) {
			if (Point.v[i] < position.v[i]) return false;
			if (Point.v[i] > (position.v[i] + size.v[i])) return false;
		}

		return true;
	}
};

typedef Bounds<int, 2> Bounds2Di;
typedef Bounds<int, 3> Bounds3Di;
typedef Bounds<float, 2> Bounds2D;
typedef Bounds<float, 3> Bounds3D;