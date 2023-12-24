#include <Camera.h>
#include <Math/Constants.h>

Camera::Camera() : FieldOfView(60.f) {
	CreatePerspectiveProjection();
}

void Camera::SetCameraType(CameraType Type) {
	this->Type = Type;
	UpdateProjectionMatrix();
}

void Camera::SetNearPlane(float NearPlane) {
	this->NearPlane = NearPlane;
	UpdateProjectionMatrix();
}

void Camera::SetFarPlane(float FarPlane) {
	this->FarPlane = FarPlane;
	UpdateProjectionMatrix();
}

void Camera::SetAspectRatio(float AspectRatio) {
	Aspect = AspectRatio;
	UpdateProjectionMatrix();
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

void Camera::UpdateProjectionMatrix() {
	switch (Type) {
		case CameraType::Perspective:
			CreatePerspectiveProjection();
			break;

		case CameraType::Orthographic:
			CreateOrthographicProjection();
			break;

		default:
			SF_ASSERT(0, "Camera has an undefined type")
	}
}