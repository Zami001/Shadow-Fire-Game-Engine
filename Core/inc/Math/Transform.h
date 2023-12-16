#pragma once

#include <Core.h>
#include <Math/Matrix4x4.h>
#include <Math/Vector.h>
#include <cmath>

#include <cstring>
#include <Logging/Log.h>
#include <MinimalCore.h>

#include <DirectXMath.h>

struct CORE_API Transform {
public:
	Transform() = default;
	~Transform() = default;

private:
	Matrix4x4 TranslationMatrix;
	Matrix4x4 RotationMatrix;
	Matrix4x4 ScaleMatrix;
	Quaternion RotationQuat;

	Matrix4x4 LocalMatrix;

	inline void RecalculateLocalMatrix() {
		LocalMatrix = TranslationMatrix * ScaleMatrix * RotationQuat.GetMatrix() * RotationMatrix;
	}

public:
	inline Matrix4x4 GetLocalMatrix() const {
		return LocalMatrix;
	}

	inline Transform& Translate(const Vector3& translation) {
		TranslationMatrix.m30 += translation.x;
		TranslationMatrix.m31 += translation.y;
		TranslationMatrix.m32 += translation.z;
		RecalculateLocalMatrix();
		return *this;
	}

	inline Transform& Scale(const Vector3& scaler) {
		ScaleMatrix.m00 *= scaler.x;
		ScaleMatrix.m11 *= scaler.y;
		ScaleMatrix.m22 *= scaler.z;
		RecalculateLocalMatrix();
		return *this;
	}

	inline Transform& Rotate(const Vector3& rotation) {
		//Matrix4x4 xRotation, yRotation, zRotation;
		//
		//xRotation.m11 = std::cos(rotation.x);
		//xRotation.m21 = std::sin(rotation.x);
		//xRotation.m12 = -std::sin(rotation.x);
		//xRotation.m22 = std::cos(rotation.x);
		//
		//yRotation.m00 = std::cos(rotation.y);
		//yRotation.m20 = std::sin(rotation.y);
		//yRotation.m02 = -std::sin(rotation.y);
		//yRotation.m22 = std::cos(rotation.y);
		//
		//zRotation.m00 = std::cos(rotation.z);
		//zRotation.m10 = std::sin(rotation.z);
		//zRotation.m01 = -std::sin(rotation.z);
		//zRotation.m11 = std::cos(rotation.z);
		//
		//RotationMatrix = zRotation * yRotation * xRotation * RotationMatrix;

		RotationQuat = RotationQuat * Quaternion(rotation);

		RecalculateLocalMatrix();
		return *this;
	}

	inline Transform& Rotate(const Quaternion& Rotator) {
		//RotationQuat = Rotator;
		RotationQuat = Rotator * RotationQuat;
		RecalculateLocalMatrix();
		return *this;
	}

	inline Vector3 operator*(const Vector3& other) const {
		Matrix4x4 local = GetLocalMatrix();
		Vector4 Vect = Vector4(other.x, other.y, other.z, 1);
		Vector4 result;

		float tmp;

		for (int row = 0; row < 4; ++row) {
			tmp = 0;

			for (int i = 0; i < 4; i++) {
				tmp += local.m[i][row] * Vect.v[i];
			}

			result.v[row] = tmp;
		}

		return Vector3(result.x, result.y, result.z) / result.w;
	}

	inline Transform& CreateProjection(float FoV, float Aspect, float NearPlane, float FarPlane) {
		float sinFov = std::sin(FoV * 0.5 * Math::Deg2Rad);
		float cosFov = std::cos(FoV * 0.5 * Math::Deg2Rad);
		float height = 1.0 / std::tan(FoV * 0.5 * Math::Deg2Rad);
		float width = height / Aspect;
		float fRange = FarPlane / (FarPlane - NearPlane);

		RotationMatrix = 
		{
			width,	0,		0,						0,
			0,		height,	0,						0,
			0,		0,		fRange,					1.f,
			0,		0,		-fRange * NearPlane,	0
		};

		RecalculateLocalMatrix();
		return *this;
	}
};