#pragma once

#include <Rendering.h>
#include <Math/Quaternion.h>
#include <Math/Vector.h>
#include <string>
#include <vector>

class RENDERING_API SkeletalAnimation {
	friend class MeshImporter;

public:
	struct RotationKey {
		double Time;
		Quaternion Rotation;

		RotationKey(double Time, Quaternion Rotation) : Time(Time), Rotation(Rotation) {}
	};
	
	struct PositionKey {
		double Time;
		Vector3 Offset;

		PositionKey(double Time, Vector3 Offset) : Time(Time), Offset(Offset) {}
	};
	
	struct ScaleKey {
		double Time;
		Vector3 Scaler;

		ScaleKey(double Time, Vector3 Scaler) : Time(Time), Scaler(Scaler) {}
	};
	
	struct BoneKeys {
		std::string BoneName;
		std::vector<RotationKey> Rotations;
		std::vector<PositionKey> Positions;
		std::vector<ScaleKey> Scales;
	};

public:
	SkeletalAnimation() = default;
	virtual ~SkeletalAnimation() = default;

	void Sample(float Time, std::vector<Matrix4x4>& outMatrix) const;

private:
	std::vector<BoneKeys> Keys;
	float Duration;
	float PlaySpeed;

public:
	inline float GetDuration() const { return Duration; }
	inline float GetPlaySpeed() const { return PlaySpeed; }
};