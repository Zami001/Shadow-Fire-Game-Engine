#pragma once

#include <Core.h>
#include <type_traits>
#include <cmath>
#include <initializer_list>
#include <format>

template<typename DataType, unsigned int Dimensions> struct Vector;

template<typename DataType, unsigned int Dimensions>
struct vec_base {
	DataType v[Dimensions];

	vec_base() {};
	vec_base(std::array<DataType, Dimensions> data) : v(data) {};
};

template<typename DataType>
struct vec_base<DataType, 2> {
	union {
		DataType v[2];
		struct { DataType x, y; };
		struct { DataType r, g; };
	};

	constexpr vec_base() : x(0), y(0) {};
	constexpr vec_base(DataType x, DataType y) : x(x), y(y) {}
};

template<typename DataType>
struct vec_base<DataType, 3> {
	union {
		DataType v[3];
		struct { DataType x, y, z; };
		struct { DataType r, g, b; };
	};

	constexpr vec_base() : x(0), y(0), z(0) {};
	constexpr vec_base(DataType x, DataType y, DataType z) : x(x), y(y), z(z) {}

	constexpr static inline Vector<DataType, 3> Cross(Vector<DataType, 3> A, Vector<DataType, 3> B) {
		return {
			(A.v[1] * B.v[2]) - (A.v[2] * B.v[1]),
			(A.v[2] * B.v[0]) - (A.v[0] * B.v[2]),
			(A.v[0] * B.v[1]) - (A.v[1] * B.v[0]),
		};
	}

	std::string ToString() const {
		return std::format("({}, {}, {})", x, y, z);
	}
};

template<typename DataType>
struct vec_base<DataType, 4> {
	union {
		DataType v[4];
		struct { DataType x, y, z, w; };
		struct { DataType r, g, b, a; };
	};

	constexpr vec_base() : x(0), y(0), z(0), w(0) {};
	constexpr vec_base(DataType x, DataType y, DataType z, DataType w) : x(x), y(y), z(z), w(w) {}

	std::string ToString() const {
		return std::format("({}, {}, {}, {})", x, y, z, w);
	}
};

template<typename DataType = float, unsigned int Dimensions = 3U>
struct Vector : public vec_base<DataType, Dimensions> {
	static_assert(std::is_arithmetic<DataType>::value, "Vectors must use a number based data type");
	static_assert(Dimensions > 1, "Vectors must have at least 2 dimensions");
	using base = vec_base<DataType, Dimensions>;

	constexpr inline Vector() : base() {}

	template<typename... Args>
	constexpr inline Vector(Args&&... args) : base(std::forward<Args>(args)...) {}

	constexpr inline Vector(DataType data[Dimensions]) {
		for (int i = 0; i < Dimensions; ++i) {
			base::v[i] = data[i];
		}
	}

	constexpr inline Vector operator+(Vector rhs) const noexcept {
		Vector vec;
		for (int i = 0; i < Dimensions; ++i) {
			vec.v[i] = base::v[i] + rhs.v[i];
		}
		return vec;
	}

	constexpr inline Vector operator-(Vector rhs) const noexcept {
		Vector vec;
		for (int i = 0; i < Dimensions; ++i) {
			vec.v[i] = base::v[i] - rhs.v[i];
		}
		return vec;
	}

	constexpr inline Vector operator/(double scaler) const {
		Vector vec;
		for (int i = 0; i < Dimensions; ++i) {
			vec.v[i] = base::v[i] / scaler;
		}
		return vec;
	}

	constexpr inline Vector operator*(double scaler) const noexcept {
		Vector vec;
		for (int i = 0; i < Dimensions; ++i) {
			vec.v[i] = base::v[i] * scaler;
		}
		return vec;
	}

	constexpr inline double SqrMagnitude() const noexcept {
		double mag = 0.0;
		for (int i = 0; i < Dimensions; ++i) {
			mag += (base::v[i] * base::v[i]);
		}
		return mag;
	}

	inline double Magnitude() const {
		return std::sqrt(SqrMagnitude());
	}

	inline Vector Normalize() {
		double scale = 1.0 / Magnitude();
		return *this * scale;
	}

	static inline Vector Lerp(Vector A, Vector B, double Alpha) noexcept {
		Vector vec;
		for (int i = 0; i < Dimensions; ++i) {
			vec.v[i] = std::lerp(A.v[i], B.v[i], Alpha);
		}
		return vec;
	}
};

static_assert(sizeof(Vector<float, 3>) == sizeof(float) * 3, "Vector has a memory size that is no longer valid");

typedef Vector<float, 2> Vector2;
typedef Vector<float, 3> Vector3;
typedef Vector<float, 4> Vector4;
typedef Vector<int, 2> Vector2i;