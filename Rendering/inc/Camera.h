#pragma once

#include <Rendering.h>
#include <Math/Matrix4x4.h>
#include <Math/Vector.h>
#include <Asserts.h>

enum class RENDERING_API CameraType {
	Perspective,
	Orthographic
};

class RENDERING_API Camera {
public:
	Camera();
	~Camera() = default;

private:
	union {
		struct { // Perspective values
			float FieldOfView;
		};

		struct { // Orthographic values
			int Size;
		};
	};

	CameraType Type = CameraType::Perspective;
	float NearPlane = 0.1f;
	float FarPlane = 1000.f;
	float Aspect = 2560.f / 1440.f;
	Matrix4x4 ProjectionMatrix;

public:
	inline CameraType GetCameraType() const { return Type; }
	inline float GetNearPlane() const { return NearPlane; }
	inline float GetFarPlane() const { return FarPlane; }
	inline float GetAspectRatio() const { return Aspect; }
	inline const Matrix4x4& GetProjectionMatrix() const { return ProjectionMatrix; }
	
	inline float GetFOV() const { 
		SF_ENSURE(Type == CameraType::Perspective, "FOV should only be used with perspective cameras")
		return FieldOfView; 
	}

	inline float GetOrthographicSize() const {
		SF_ENSURE(Type == CameraType::Orthographic, "Size should only be used with orthographic cameras")
		return Size;
	}

	void SetCameraType(CameraType Type);
	void SetNearPlane(float NearPlane);
	void SetFarPlane(float FarPlane);
	void SetAspectRatio(float AspectRatio);

private:
	void CreateOrthographicProjection();
	void CreatePerspectiveProjection();
	void UpdateProjectionMatrix();
};