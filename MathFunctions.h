#pragma once

namespace MathFunctions {
	struct Vector2 { float x, y; };
	struct Vector3 { float x, y, z; };
	struct Vector4 { float x, y, z, w; };
	struct Matrix4x4 { float m[4][4]; };

	Matrix4x4 MakeIdentity4x4();

	double cot(double angle);

	Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

	Matrix4x4 MakeScaleMatrix(const Vector3& scale);

	Matrix4x4 MakeRotateXMatrix(float radius);

	Matrix4x4 MakeRotateYMatrix(float radius);

	Matrix4x4 MakeRotateZMatrix(float radius);

	Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

	Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

	Matrix4x4 MakePrespectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);

	Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);

	Matrix4x4 Inverse(const Matrix4x4& m);

	float Length(const Vector3& v);

	Vector3 Normalize(const Vector3& v);
}