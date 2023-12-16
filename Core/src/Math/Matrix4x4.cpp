#include <Math/Matrix4x4.h>
#include <DirectXMath.h>
#include <xmmintrin.h>
#include <Math/Quaternion.h>


// hack: definitely should be using custom inverse method, but currently hijacking the one in directx math
Matrix4x4 Matrix4x4::Inverse() const {
	DirectX::XMVECTOR vec;
	auto mat = DirectX::XMMatrixInverse(&vec, *reinterpret_cast<const DirectX::XMMATRIX*>(this));

	return *reinterpret_cast<Matrix4x4*>(&mat);
}

Matrix4x4& Matrix4x4::operator*=(const Matrix4x4& other) {
	operator=((*this) * other);

	return *this;
}

void Matrix4x4::ExtractTransformComponents(Vector3& outTranslation, Vector3& outScale, Matrix4x4& outRotation) {
	outTranslation = { m03, m13, m23 };

	outScale.x = Vector3(m00, m10, m20).Magnitude();
	outScale.y = Vector3(m01, m11, m21).Magnitude();
	outScale.z = Vector3(m02, m12, m22).Magnitude();

	outRotation = {
		m00 / outScale.x, m01 / outScale.y, m02 / outScale.z, 0,
		m10 / outScale.x, m11 / outScale.y, m12 / outScale.z, 0,
		m20 / outScale.x, m21 / outScale.y, m22 / outScale.z, 0,
		0, 0, 0, 1
	};
}