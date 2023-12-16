#include <Math/Quaternion.h>

#include <DirectXMath.h>

Matrix4x4 Quaternion::GetMatrix() const {
	Matrix4x4 mat;

	mat.m00 = 1.0 - 2.0 * (y * y + z * z);
	mat.m10 = 2.0 * (x * y - z * w);
	mat.m20 = 2.0 * (x * z + y * w);
	mat.m30 = 0;
	mat.m01 = 2.0 * (x * y + z * w);
	mat.m11 = 1.0 - 2.0 * (x * x + z * z);
	mat.m21 = 2.0 * (y * z - x * w);
	mat.m31 = 0;
	mat.m02 = 2.0 * (x * z - y * w);
	mat.m12 = 2.0 * (y * z + x * w);
	mat.m22 = 1.0 - 2.0 * (x * x + y * y);
	mat.m32 = 0;
	mat.m03 = 0;
	mat.m13 = 0;
	mat.m23 = 0;
	mat.m33 = 1;

	return mat;
}

// implementation taken from assimp library
Quaternion Quaternion::Slerp(const Quaternion& left, const Quaternion& right, float value) {
    // calc cosine theta
    float cosom = left.x * right.x + left.y * right.y + left.z * right.z + left.w * right.w;

    // adjust signs (if necessary)
    Quaternion end = right;
    if (cosom < 0.0f) {
        cosom = -cosom;
        end.x = -end.x;   // Reverse all signs
        end.y = -end.y;
        end.z = -end.z;
        end.w = -end.w;
    }

    // Calculate coefficients
    float sclp, sclq;
    if ((1.0f - cosom) > 0.0001f) // 0.0001 -> some epsillon
    {
        // Standard case (slerp)
        float omega, sinom;
        omega = std::acos(cosom); // extract theta from dot product's cos theta
        sinom = std::sin(omega);
        sclp = std::sin((1.0f - value) * omega) / sinom;
        sclq = std::sin(value * omega) / sinom;
    } else {
        // Very close, do linear interp (because it's faster)
        sclp = 1.0f - value;
        sclq = value;
    }

    Quaternion result;
    result.x = sclp * left.x + sclq * end.x;
    result.y = sclp * left.y + sclq * end.y;
    result.z = sclp * left.z + sclq * end.z;
    result.w = sclp * left.w + sclq * end.w;

    return result;
}