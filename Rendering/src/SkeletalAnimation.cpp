#include <SkeletalAnimation.h>
#include <MinimalCore.h>

void SkeletalAnimation::Sample(float Time, std::vector<Matrix4x4>& outMatrix) const {
	outMatrix.clear();
	outMatrix.reserve(Keys.size());

	for (int i = 0; i < Keys.size(); ++i) {
		Vector3 position = Keys[i].Positions[Keys[i].Positions.size() - 1].Offset;
		Vector3 scale = Keys[i].Scales[Keys[i].Scales.size() - 1].Scaler;
		Quaternion rotation = Keys[i].Rotations[Keys[i].Rotations.size() - 1].Rotation;

		for (int posKey = 1; posKey < Keys[i].Positions.size(); ++posKey) {
			if (Time < Keys[i].Positions[posKey].Time) {
				float delta = Keys[i].Positions[posKey].Time - Keys[i].Positions[posKey - 1].Time;
				float offset = Keys[i].Positions[posKey - 1].Time;
				float value = (Time - offset) / delta;
				position = Vector3::Lerp(Keys[i].Positions[posKey - 1].Offset, Keys[i].Positions[posKey].Offset, value);
				break;
			}
		}

		for (int scaleKey = 1; scaleKey < Keys[i].Scales.size(); ++scaleKey) {
			if (Time < Keys[i].Scales[scaleKey].Time) {
				float delta = Keys[i].Rotations[scaleKey].Time - Keys[i].Rotations[scaleKey - 1].Time;
				float offset = Keys[i].Rotations[scaleKey - 1].Time;
				float value = (Time - offset) / delta;
				scale = Vector3::Lerp(Keys[i].Scales[scaleKey - 1].Scaler, Keys[i].Scales[scaleKey].Scaler, value);
				break;
			}
		}

		for (int rotKey = 1; rotKey < Keys[i].Rotations.size(); ++rotKey) {
			if (Time < Keys[i].Rotations[rotKey].Time) {
				float delta = Keys[i].Rotations[rotKey].Time - Keys[i].Rotations[rotKey - 1].Time;
				float offset = Keys[i].Rotations[rotKey - 1].Time;
				float value = (Time - offset) / delta;
				rotation = Quaternion::Slerp(Keys[i].Rotations[rotKey - 1].Rotation, Keys[i].Rotations[rotKey].Rotation, value);
				break;
			}
		}
		
		Matrix4x4 Transformation;

		Transformation = rotation.GetMatrix();
		Transformation.m00 *= scale.x;
		Transformation.m10 *= scale.x;
		Transformation.m20 *= scale.x;
		Transformation.m01 *= scale.y;
		Transformation.m11 *= scale.y;
		Transformation.m21 *= scale.y;
		Transformation.m02 *= scale.z;
		Transformation.m12 *= scale.z;
		Transformation.m22 *= scale.z;
		Transformation.m30 = position.x;
		Transformation.m31 = position.y;
		Transformation.m32 = position.z;

		outMatrix.push_back(Transformation);

		if (i == 0) {
			Matrix4x4 outRot;
			Vector3 outScale, outTranslation;
			Transformation.ExtractTransformComponents(outTranslation, outScale, outRot);
			//SF_LOG(LogTest, Log, "%s", outScale.ToString().c_str())
		}
	}
}