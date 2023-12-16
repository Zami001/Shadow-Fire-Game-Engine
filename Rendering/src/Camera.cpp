#include <Camera.h>
#include <Math/Constants.h>

Camera::Camera() : FieldOfView(60.f) {
	CreatePerspectiveProjection();
}

void Camera::CreateOrthographicProjection() {
}

void Camera::CreatePerspectiveProjection() {
	float sinFov = std::sin(GetFOV() * 0.5 * Math::Deg2Rad);
	float cosFov = std::cos(GetFOV() * 0.5 * Math::Deg2Rad);
	float height = 1.0 / std::tan(GetFOV() * 0.5 * Math::Deg2Rad);
	float width = height / Aspect;
	float fRange = FarPlane / (FarPlane - NearPlane);

	ProjectionMatrix =
	{
		width,	0,				0,						0,
		0,				height,	0,						0,
		0,				0,				fRange,					1.f,
		0,				0,				-fRange * NearPlane,	0
	};
}