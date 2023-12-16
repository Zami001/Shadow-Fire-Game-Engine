#pragma once

#include <Core.h>
#include <Math/Vector.h>

struct CORE_API alignas(16) Matrix4x4 {
public:
	union {
		float s[16];
		float m[4][4];
		struct {
			float m00, m01, m02, m03;
			float m10, m11, m12, m13;
			float m20, m21, m22, m23;
			float m30, m31, m32, m33;
		};
	};

public:
	constexpr Matrix4x4() : m00(1), m01(0), m02(0), m03(0), m10(0), m11(1), m12(0), m13(0), m20(0), m21(0), m22(1), m23(0), m30(0), m31(0), m32(0), m33(1) {}
	constexpr Matrix4x4(const float val[16]) : m00(val[0]), m01(val[1]), m02(val[2]), m03(val[3]), m10(val[4]), m11(val[5]), m12(val[6]), m13(val[7]), m20(val[8]), m21(val[9]), m22(val[10]), m23(val[11]), m30(val[12]), m31(val[13]), m32(val[14]), m33(val[15]) {}
	constexpr Matrix4x4(const float m00, const float m01, const float m02, const float m03, const float m10, const float m11, const float m12, const float m13, const float m20, const float m21, const float m22, const float m23, const float m30, const float m31, const float m32, const float m33)
		: m00(m00), m01(m01), m02(m02), m03(m03), m10(m10), m11(m11), m12(m12), m13(m13), m20(m20), m21(m21), m22(m22), m23(m23), m30(m30), m31(m31), m32(m32), m33(m33) {}

	static constexpr Matrix4x4 Identity() { return Matrix4x4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1); }

	constexpr Matrix4x4 Transpose() const {
		return Matrix4x4({
			m00, m10, m20, m30,
			m01, m11, m21, m31,
			m02, m12, m22, m32,
			m03, m13, m23, m33
		});
	}

	Matrix4x4 Inverse() const;

	// operator overloads
	constexpr Matrix4x4 operator*(const Matrix4x4& other) const noexcept {
		Matrix4x4 result;

		float tmp;

		for (int col = 0; col < 4; ++col) {
			for (int row = 0; row < 4; ++row) {
				tmp = 0;

				for (int i = 0; i < 4; i++) {
					tmp += m[i][row] * other.m[col][i];
				}

				result.m[col][row] = tmp;
			}
		}

		return result;
	}

	Matrix4x4& operator*=(const Matrix4x4& other);

	constexpr Vector4 operator*(const Vector4& other) const noexcept {
		Vector4 result;

		float tmp;

		for (int row = 0; row < 4; ++row) {
			tmp = 0;

			for (int i = 0; i < 4; i++) {
				tmp += m[i][row] * other.v[i];
			}

			result.v[row] = tmp;
		}

		return result;
	}

	void ExtractTransformComponents(Vector3& outTranslation, Vector3& outScale, Matrix4x4& outRotation);
};

static_assert(sizeof(Matrix4x4) == sizeof(float) * 16, "Matrix4x4 data size must match that expected by GPUs");
static_assert(alignof(Matrix4x4) == 16, "GPUs expect a memory alignment of 16 for matrix data");