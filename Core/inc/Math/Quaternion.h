#pragma once

#include <Core.h>
#include <Math/Matrix4x4.h>
#include <Math/Vector.h>

struct CORE_API Quaternion {
private:
	// most of the quaternion functions are pulled from the quaternion type in assimp. 
	float x, y, z, w;

public:
	constexpr Quaternion() : x(0), y(0), z(0), w(1) {}
	constexpr Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

	/// @brief Construct quaternion from Eular angles
	/// @param EularAngles Pitch Yaw Roll values
	inline Quaternion(Vector3 EularAngles) {
		const float fSinPitch(std::sin(EularAngles.y * 0.5));
		const float fCosPitch(std::cos(EularAngles.y * 0.5));
		const float fSinYaw(std::sin(EularAngles.z * 0.5));
		const float fCosYaw(std::cos(EularAngles.z * 0.5));
		const float fSinRoll(std::sin(EularAngles.x * 0.5));
		const float fCosRoll(std::cos(EularAngles.x * 0.5));
		const float fCosPitchCosYaw(fCosPitch * fCosYaw);
		const float fSinPitchSinYaw(fSinPitch * fSinYaw);
		x = fSinRoll * fCosPitchCosYaw - fCosRoll * fSinPitchSinYaw;
		y = fCosRoll * fSinPitch * fCosYaw + fSinRoll * fCosPitch * fSinYaw;
		z = fCosRoll * fCosPitch * fSinYaw - fSinRoll * fSinPitch * fCosYaw;
		w = fCosRoll * fCosPitchCosYaw + fSinRoll * fSinPitchSinYaw;
	}

	inline Quaternion(Vector3 axis, double angle) noexcept {
		axis.Normalize();

		const float sin_a = std::sin(angle / 2);
		const float cos_a = std::cos(angle / 2);
		x = axis.x * sin_a;
		y = axis.y * sin_a;
		z = axis.z * sin_a;
		w = cos_a;
	}
	
	Matrix4x4 GetMatrix() const;

	constexpr inline Quaternion operator*(const Quaternion& other) const noexcept {
		return Quaternion {
			w * other.x + x * other.w + y * other.z - z * other.y,
			w * other.y + y * other.w + z * other.x - x * other.z,
			w * other.z + z * other.w + x * other.y - y * other.x,
			w * other.w - x * other.x - y * other.y - z * other.z
		};
	}

	static Quaternion Slerp(const Quaternion& left, const Quaternion& right, float value);
};