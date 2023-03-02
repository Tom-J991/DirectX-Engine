#pragma once

#define _USE_MATH_DEFINES 
#include <math.h>

namespace Math
{
	inline float DegreesToRadians(float d)
	{
		return d * ((float)M_PI / 180.0f);
	}
	inline float RadiansToDegrees(float r)
	{
		return r * (180.0f / (float)M_PI);
	}

	// Vectors
	struct Vector2F
	{
		// Anonymous Struct // Allows interchangable usage of array or individual variables.
		union { struct { float x, y; }; float v[2]; };

		// Constructors
		Vector2F()
			: x(0.0f), y(0.0f)
		{ }
		Vector2F(float x, float y)
			: x(x), y(y)
		{ }

		// Methods
		float MagnitudeSqr() { return (x * x + y * y); }
		float Magnitude() { return sqrtf(MagnitudeSqr()); }
		Vector2F Normalise() { return (Magnitude() != 0.0f) ? (*this / Magnitude()) : *this; };

		float Dot(Vector2F v) { return (x * v.x + y * v.y); };
		float Cross(Vector2F v) { return (x * v.y) - (y * v.x); }
		float Angle(Vector2F v) { float d = Normalise().Dot(v.Normalise()); return acosf(d); }
		float Distance(Vector2F v) { Vector2F diff = *this - v; return diff.Magnitude(); }

		// Operator Overloads
		friend Vector2F operator+(const Vector2F& a, const Vector2F& b) { return Vector2F(a.x + b.x, a.y + b.y); }
		friend Vector2F operator-(const Vector2F& a, const Vector2F& b) { return Vector2F(a.x - b.x, a.y - b.y); }
		friend Vector2F operator*(const Vector2F& a, const float& b) { return Vector2F(a.x * b, a.y * b); } // Left Hand Side
		friend Vector2F operator*(const float& a, const Vector2F& b) { return Vector2F(b.x * a, b.y * a); } // Right Hand Side
		friend Vector2F operator/(const Vector2F& a, const float& b) { return Vector2F(a.x / b, a.y / b); } // Left Hand Side
		friend Vector2F operator/(const float& a, const Vector2F& b) { return Vector2F(b.x / a, b.y / a); } // Right Hand Side

		friend Vector2F operator-(const Vector2F& a) { return (a * -1.0f); } // Negate

		Vector2F& operator+=(const Vector2F& v) { this->x += v.x; this->y += v.y; return *this; }
		Vector2F& operator-=(const Vector2F& v) { this->x -= v.x; this->y -= v.y; return *this; }
		Vector2F& operator*=(const float& f) { this->x *= f; this->y *= f; return *this; }
		Vector2F& operator/=(const float& f) { this->x /= f; this->y /= f; return *this; }

	};
	struct Vector3F
	{
		// Anonymous Struct // Allows interchangable usage of array or individual variables.
		union { struct { float x, y, z; }; float v[3]; };

		// Constructors
		Vector3F()
			: x(0.0f), y(0.0f), z(0.0f)
		{ }
		Vector3F(float x, float y, float z)
			: x(x), y(y), z(z)
		{ }

		// Methods
		float MagnitudeSqr() { return (x * x + y * y + z * z); }
		float Magnitude() { return sqrtf(MagnitudeSqr()); }
		Vector3F Normalise() { return (Magnitude() != 0.0f) ? (*this / Magnitude()) : *this; };

		float Dot(Vector3F v) { return (x * v.x + y * v.y + z * v.z); };
		Vector3F Cross(Vector3F v) { return Vector3F(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x); }
		float Angle(Vector3F v) { float d = Normalise().Dot(v.Normalise()); return acosf(d); }
		float Distance(Vector3F v) { Vector3F diff = *this - v; return diff.Magnitude(); }

		// Operator Overloads
		friend Vector3F operator+(const Vector3F& a, const Vector3F& b) { return Vector3F(a.x + b.x, a.y + b.y, a.z + b.z); }
		friend Vector3F operator-(const Vector3F& a, const Vector3F& b) { return Vector3F(a.x - b.x, a.y - b.y, a.z - b.z); }
		friend Vector3F operator*(const Vector3F& a, const float& b) { return Vector3F(a.x * b, a.y * b, a.z * b); } // Left Hand Side
		friend Vector3F operator*(const float& a, const Vector3F& b) { return Vector3F(b.x * a, b.y * a, b.z * a); } // Right Hand Side
		friend Vector3F operator/(const Vector3F& a, const float& b) { return Vector3F(a.x / b, a.y / b, a.z / b); } // Left Hand Side
		friend Vector3F operator/(const float& a, const Vector3F& b) { return Vector3F(b.x / a, b.y / a, b.z / a); } // Right Hand Side

		friend Vector3F operator-(const Vector3F& a) { return (a * -1.0f); } // Negate

		Vector3F& operator+=(const Vector3F& v) { this->x += v.x; this->y += v.y; this->z += v.z; return *this; }
		Vector3F& operator-=(const Vector3F& v) { this->x -= v.x; this->y -= v.y; this->z -= v.z; return *this; }
		Vector3F& operator*=(const float& f) { this->x *= f; this->y *= f; this->z *= f; return *this; }
		Vector3F& operator/=(const float& f) { this->x /= f; this->y /= f; this->z *= f; return *this; }

	};
	struct Vector4F
	{
		// Anonymous Struct // Allows interchangable usage of array or individual variables.
		union { struct { float x, y, z, w; }; float v[4]; };

		// Constructors
		Vector4F() 
			: x(0.0f), y(0.0f), z(0.0f), w(0.0f)
		{ }
		Vector4F(float x, float y, float z, float w)
			: x(x), y(y), z(z), w(w)
		{ }

		// Methods
		float MagnitudeSqr() { return (x * x + y * y + z * z + w * w); }
		float Magnitude() { return sqrtf(MagnitudeSqr()); }
		Vector4F Normalise() { return (Magnitude() != 0.0f) ? (*this / Magnitude()) : *this; };

		float Dot(Vector4F v) { return (x * v.x + y * v.y + z * v.z + w * v.w); };
		Vector4F Cross(Vector4F v) { return Vector4F(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x, 0.0f); }
		float Angle(Vector4F v) { float d = Normalise().Dot(v.Normalise()); return acosf(d); }
		float Distance(Vector4F v) { Vector4F diff = *this - v; return diff.Magnitude(); }

		// Operator Overloads
		friend Vector4F operator+(const Vector4F& a, const Vector4F& b) { return Vector4F(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w); }
		friend Vector4F operator-(const Vector4F& a, const Vector4F& b) { return Vector4F(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w); }
		friend Vector4F operator*(const Vector4F& a, const float& b) { return Vector4F(a.x * b, a.y * b, a.z * b, a.w * b); } // Left Hand Side
		friend Vector4F operator*(const float& a, const Vector4F& b) { return Vector4F(b.x * a, b.y * a, b.z * a, b.w * a); } // Right Hand Side
		friend Vector4F operator/(const Vector4F& a, const float& b) { return Vector4F(a.x / b, a.y / b, a.z / b, a.w / b); } // Left Hand Side
		friend Vector4F operator/(const float& a, const Vector4F& b) { return Vector4F(b.x / a, b.y / a, b.z / a, b.w / a); } // Right Hand Side

		friend Vector4F operator-(const Vector4F& a) { return (a * -1.0f); } // Negate

		Vector4F& operator+=(const Vector4F& v) { this->x += v.x; this->y += v.y; this->z += v.z; this->w += v.w; return *this; }
		Vector4F& operator-=(const Vector4F& v) { this->x -= v.x; this->y -= v.y; this->z -= v.z; this->w -= v.w; return *this; }
		Vector4F& operator*=(const float& f) { this->x *= f; this->y *= f; this->z *= f; this->w *= f; return *this; }
		Vector4F& operator/=(const float& f) { this->x /= f; this->y /= f; this->z *= f; this->w /= f; return *this; }

	};

	// Matrices
	struct Matrix3F
	{
		// Anonymous Struct // Allows interchangable usage of array or individual variables (Shares Memory).
		union { 
			struct {
				float m00, m01, m02;
				float m10, m11, m12;
				float m20, m21, m22;
			}; 
			float m[3][3];
		};

		// Constructors
		Matrix3F()
			: m00(0.0f), m01(0.0f), m02(0.0f)
			, m10(0.0f), m11(0.0f), m12(0.0f)
			, m20(0.0f), m21(0.0f), m22(0.0f)
		{ }
		Matrix3F(float i)
			: m00(i), m01(0.0f), m02(0.0f)
			, m10(0.0f), m11(i), m12(0.0f)
			, m20(0.0f), m21(0.0f), m22(i)
		{ }
		Matrix3F(float m00, float m01, float m02,
				float m10, float m11, float m12,
				float m20, float m21, float m22)
			: m00(m00), m01(m01), m02(m02)
			, m10(m10), m11(m11), m12(m12)
			, m20(m20), m21(m21), m22(m22)
		{ }
		Matrix3F(Vector3F x, Vector3F y, Vector3F z)
			: m00(x.x), m01(y.x), m02(z.x)
			, m10(x.y), m11(y.y), m12(z.y)
			, m20(x.z), m21(y.z), m22(z.z)
		{ }

		// Methods
			// Scale
		Matrix3F SetScaled(float x, float y, float z)
		{
			m00 = x; m01 = 0.0f; m02 = 0.0f;
			m10 = 0.0f; m11 = y; m12 = 0.0f;
			m20 = 0.0f; m21 = 0.0f; m22 = z;
			return *this;
		}
		Matrix3F SetScaled(Vector3F v) { return SetScaled(v.x, v.y, v.z); }
		Matrix3F SetScaled(Vector4F v) { return SetScaled(v.x, v.y, v.z); }
		Matrix3F Scale(float x, float y, float z)
		{
			Matrix3F m = Matrix3F(1.0f);
			m.SetScaled(x, y, z);
			*this = *this * m;
			return *this;
		}
		Matrix3F Scale(Vector3F v) { return Scale(v.x, v.y, v.z); }
		Matrix3F Scale(Vector4F v) { return Scale(v.x, v.y, v.z); }

			// Rotation
		Matrix3F SetRotateX(float radians)
		{
			m00 = 1.0f; m01 = 0.0f; m02 = 0.0f;
			m10 = 0.0f; m11 = cosf(radians); m12 = sinf(radians);
			m20 = 0.0f; m21 = -sinf(radians); m22 = cosf(radians);
			return *this;
		}
		Matrix3F RotateX(float radians)
		{
			Matrix3F m = Matrix3F(1.0f);
			m.SetRotateX(radians);
			*this = *this * m;
			return *this;
		}
		Matrix3F SetRotateY(float radians)
		{
			m00 = cosf(radians); m01 = 0.0f; m02 = -sinf(radians);
			m10 = 0.0f; m11 = 1.0f; m12 = 0.0f;
			m20 = sinf(radians); m21 = 0.0f; m22 = cosf(radians);
			return *this;
		}
		Matrix3F RotateY(float radians)
		{
			Matrix3F m = Matrix3F(1.0f);
			m.SetRotateY(radians);
			*this = *this * m;
			return *this;
		}
		Matrix3F SetRotateZ(float radians)
		{
			m00 = cosf(radians); m01 = sinf(radians); m02 = 0.0f;
			m10 = -sinf(radians); m11 = cosf(radians); m12 = 0.0f;
			m20 = 0.0f; m21 = 0.0f; m22 = 1.0f;
			return *this;
		}
		Matrix3F RotateZ(float radians)
		{
			Matrix3F m = Matrix3F(1.0f);
			m.SetRotateZ(radians);
			*this = *this * m;
			return *this;
		}

			// Translation
		Matrix3F SetTranslation(float x, float y) { m02 = x; m12 = y; return *this; }
		Matrix3F SetTranslation(Vector2F v) { return SetTranslation(v.x, v.y); }
		Matrix3F SetTranslation(Vector3F v) { return SetTranslation(v.x, v.y); }
		Matrix3F SetTranslation(Vector4F v) { return SetTranslation(v.x, v.y); }
		Matrix3F Translate(float x, float y) { m02 += x; m12 += y; return *this; }

		Matrix3F SetEuler(float pitch, float yaw, float roll)
		{
			Matrix3F x = Matrix3F(1.0f);
			Matrix3F y = Matrix3F(1.0f);
			Matrix3F z = Matrix3F(1.0f);
			x.SetRotateX(pitch);
			y.SetRotateY(yaw);
			z.SetRotateZ(roll);
			*this = z * y * x;
			return *this;
		}

		// Operator Overloads
		friend Matrix3F operator*(const Matrix3F& a, const Matrix3F& b) 
		{ 
			return Matrix3F(
				// First Row
				a.m00*b.m00 + a.m10*b.m01 + a.m20*b.m02, // m00
				a.m01*b.m00 + a.m11*b.m01 + a.m21*b.m02, // m01
				a.m02*b.m00 + a.m12*b.m01 + a.m22*b.m02, // m02
				// Second Row
 				a.m00*b.m10 + a.m10*b.m11 + a.m20*b.m12, // m10
				a.m01*b.m10 + a.m11*b.m11 + a.m21*b.m12, // m11
				a.m02*b.m10 + a.m12*b.m11 + a.m22*b.m12, // m12
				// Third Row
 				a.m00*b.m20 + a.m10*b.m21 + a.m20*b.m22, // m20
				a.m01*b.m20 + a.m11*b.m21 + a.m21*b.m22, // m21
				a.m02*b.m20 + a.m12*b.m21 + a.m22*b.m22  // m22
			);
		}
		friend Vector3F operator*(const Matrix3F& a, const Vector3F& b)
		{ 
			return Vector3F(b.x*a.m00 + b.y*a.m10 + b.z*a.m20,
							b.x*a.m01 + b.y*a.m11 + b.z*a.m21,
							b.x*a.m02 + b.y*a.m12 + b.z*a.m22);
		}

	};
	struct Matrix4F
	{
		// Anonymous Struct // Allows interchangable usage of array or individual variables (Shares Memory).
		union {
			struct {
				float m00, m01, m02, m03;
				float m10, m11, m12, m13;
				float m20, m21, m22, m23;
				float m30, m31, m32, m33;
			};
			float m[4][4];
		};

		// Constructors
		Matrix4F()
			: m00(0.0f), m01(0.0f), m02(0.0f), m03(0.0f)
			, m10(0.0f), m11(0.0f), m12(0.0f), m13(0.0f)
			, m20(0.0f), m21(0.0f), m22(0.0f), m23(0.0f)
			, m30(0.0f), m31(0.0f), m32(0.0f), m33(0.0f)
		{ }
		Matrix4F(float i)
			: m00(i), m01(0.0f), m02(0.0f), m03(0.0f)
			, m10(0.0f), m11(i), m12(0.0f), m13(0.0f)
			, m20(0.0f), m21(0.0f), m22(i), m23(0.0f)
			, m30(0.0f), m31(0.0f), m32(0.0f), m33(i)
		{ }
		Matrix4F(float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33)
			: m00(m00), m01(m01), m02(m02), m03(m03)
			, m10(m10), m11(m11), m12(m12), m13(m13)
			, m20(m20), m21(m21), m22(m22), m23(m23)
			, m30(m30), m31(m31), m32(m32), m33(m33)
		{ }
		Matrix4F(Vector4F x, Vector4F y, Vector4F z, Vector4F w)
			: m00(x.x), m01(y.x), m02(z.x), m03(w.x)
			, m10(x.y), m11(y.y), m12(z.y), m13(w.y)
			, m20(x.z), m21(y.z), m22(z.z), m23(w.z)
			, m30(x.w), m31(y.w), m32(z.w), m33(w.w)
		{ }

		// Methods
			// Scale
		Matrix4F SetScaled(float x, float y, float z)
		{
			m00 = x; m01 = 0.0f; m02 = 0.0f; m03 = 0.0f;
			m10 = 0.0f; m11 = y; m12 = 0.0f; m13 = 0.0f;
			m20 = 0.0f; m21 = 0.0f; m22 = z; m23 = 0.0f;
			m30 = 0.0f; m31 = 0.0f; m32 = 0.0f; m33 = 1.0f;
			return *this;
		}
		Matrix4F SetScaled(Vector3F v) { return SetScaled(v.x, v.y, v.z); }
		Matrix4F SetScaled(Vector4F v) { return SetScaled(v.x, v.y, v.z); }
		Matrix4F Scale(float x, float y, float z)
		{
			Matrix4F m = Matrix4F(1.0f);
			m.SetScaled(x, y, z);
			*this = *this * m;
			return *this;
		}
		Matrix4F Scale(Vector3F v) { return Scale(v.x, v.y, v.z); }
		Matrix4F Scale(Vector4F v) { return Scale(v.x, v.y, v.z); }

			// Rotation
		Matrix4F SetRotateX(float radians)
		{
			m00 = 1.0f; m01 = 0.0f; m02 = 0.0f; m03 = 0.0f;
			m10 = 0.0f; m11 = cosf(radians); m12 = sinf(radians); m13 = 0.0f;
			m20 = 0.0f; m21 = -sinf(radians); m22 = cosf(radians); m23 = 0.0f;
			m30 = 0.0f; m31 = 0.0f; m32 = 0.0f; m33 = 1.0f;
			return *this;
		}
		Matrix4F RotateX(float radians)
		{
			Matrix4F m = Matrix4F(1.0f);
			m.SetRotateX(radians);
			*this = *this * m;
			return *this;
		}
		Matrix4F SetRotateY(float radians)
		{
			m00 = cosf(radians); m01 = 0.0f; m02 = -sinf(radians); m03 = 0.0f;
			m10 = 0.0f; m11 = 1.0f; m12 = 0.0f; m13 = 0.0f;
			m20 = sinf(radians); m21 = 0.0f; m22 = cosf(radians); m23 = 0.0f;
			m30 = 0.0f; m31 = 0.0f; m32 = 0.0f; m33 = 1.0f;
			return *this;
		}
		Matrix4F RotateY(float radians)
		{
			Matrix4F m = Matrix4F(1.0f);
			m.SetRotateY(radians);
			*this = *this * m;
			return *this;
		}
		Matrix4F SetRotateZ(float radians)
		{
			m00 = cosf(radians); m01 = sinf(radians); m02 = 0.0f; m03 = 0.0f;
			m10 = -sinf(radians); m11 = cosf(radians); m12 = 0.0f; m13 = 0.0f;
			m20 = 0.0f; m21 = 0.0f; m22 = 1.0f; m23 = 0.0f;
			m30 = 0.0f; m31 = 0.0f; m32 = 0.0f; m33 = 1.0f;
			return *this;
		}
		Matrix4F RotateZ(float radians)
		{
			Matrix4F m = Matrix4F(1.0f);
			m.SetRotateZ(radians);
			*this = *this * m;
			return *this;
		}

		Matrix4F SetEuler(float pitch, float yaw, float roll)
		{
			Matrix4F x = Matrix4F(1.0f);
			Matrix4F y = Matrix4F(1.0f);
			Matrix4F z = Matrix4F(1.0f);
			x.SetRotateX(pitch);
			y.SetRotateY(yaw);
			z.SetRotateZ(roll);
			*this = z * y * x;
			return *this;
		}

			// Translation
		Matrix4F SetTranslation(float x, float y, float z) { m03 = x; m13 = y; m23 = z; m33 = 1.0f; return *this; }
		Matrix4F SetTranslation(Vector3F v) { return SetTranslation(v.x, v.y, v.z); }
		Matrix4F SetTranslation(Vector4F v) { return SetTranslation(v.x, v.y, v.z); }
		Matrix4F Translate(float x, float y, float z) { m03 += x; m13 += y; m23 += z; return *this; }
		Matrix4F Translate(Vector3F v) { return Translate(v.x, v.y, v.z); }
		Matrix4F Translate(Vector4F v) { return Translate(v.x, v.y, v.z); }

			// View
		Matrix4F LookAt(Vector3F position, Vector3F target, Vector3F up)
		{
			// TODO:
			return *this;
		}

			// Projection
		Matrix4F Perspective(float aspect, float fovRadians, float zNear, float zFar)
		{
			float yScale = tanf(0.5f * ((float)M_PI - fovRadians));
			float xScale = yScale / aspect;
			float zRangeInverse = 1.0f / (zNear - zFar);
			float zScale = zFar * zRangeInverse;
			float zTranslation = zFar * zNear * zRangeInverse;

			m00 = xScale; m01 = 0.0f; m02 = 0.0f; m03 = 0.0f;
			m10 = 0.0f; m11 = yScale; m12 = 0.0f; m13 = 0.0f;
			m20 = 0.0f; m21 = 0.0f; m22 = zScale; m23 = zTranslation;
			m30 = 0.0f; m31 = 0.0f; m32 = -1.0f; m33 = 0.0f;
			return *this;
		}
		Matrix4F Orthographic(float scale, float left, float right, float bottom, float top, float zNear, float zFar)
		{
			float xScale = scale / (right - left);
			float yScale = scale / (top - bottom);
			float zScale = scale / (zNear - zFar);

			//float xTranslation = -(right + left) / (right - left);
			float xTranslation = 0.0f;
			//float yTranslation = -(top + bottom) / (top - bottom);
			float yTranslation = 0.0f;
			float zTranslation = zNear/(zNear-zFar);

			m00 = xScale; m01 = 0.0f; m02 = 0.0f; m03 = 0.0f;
			m10 = 0.0f; m11 = yScale; m12 = 0.0f; m13 = 0.0f;
			m20 = 0.0f; m21 = 0.0f; m22 = zScale; m23 = 0.0f;
			m30 = xTranslation; m31 = yTranslation; m32 = zTranslation; m33 = 1.0f;
			return *this;
		}

		// Operator Overloads
		friend Matrix4F operator*(const Matrix4F& a, const Matrix4F& b)
		{
			return Matrix4F(
				// First Row
				a.m00*b.m00 + a.m10*b.m01 + a.m20*b.m02 + a.m30*b.m03, // m00
				a.m01*b.m00 + a.m11*b.m01 + a.m21*b.m02 + a.m31*b.m03, // m01
				a.m02*b.m00 + a.m12*b.m01 + a.m22*b.m02 + a.m32*b.m03, // m02
				a.m03*b.m00 + a.m13*b.m01 + a.m23*b.m02 + a.m33*b.m03, // m03
				// Second Row
				a.m00*b.m10 + a.m10*b.m11 + a.m20*b.m12 + a.m30*b.m13, // m10
				a.m01*b.m10 + a.m11*b.m11 + a.m21*b.m12 + a.m31*b.m13, // m11
				a.m02*b.m10 + a.m12*b.m11 + a.m22*b.m12 + a.m32*b.m13, // m12
				a.m03*b.m10 + a.m13*b.m11 + a.m23*b.m12 + a.m33*b.m13, // m13
				// Third Row
				a.m00*b.m20 + a.m10*b.m21 + a.m20*b.m22 + a.m30*b.m23, // m20
				a.m01*b.m20 + a.m11*b.m21 + a.m21*b.m22 + a.m31*b.m23, // m21
				a.m02*b.m20 + a.m12*b.m21 + a.m22*b.m22 + a.m32*b.m23, // m22
				a.m03*b.m20 + a.m13*b.m21 + a.m23*b.m22 + a.m33*b.m23, // m23
				// Fourth Row
				a.m00*b.m30 + a.m10*b.m31 + a.m20*b.m32 + a.m30*b.m33, // m30
				a.m01*b.m30 + a.m11*b.m31 + a.m21*b.m32 + a.m31*b.m33, // m31
				a.m02*b.m30 + a.m12*b.m31 + a.m22*b.m32 + a.m32*b.m33, // m32
				a.m03*b.m30 + a.m13*b.m31 + a.m23*b.m32 + a.m33*b.m33  // m33
			);
		}
		friend Vector4F operator*(const Matrix4F& a, const Vector4F& b)
		{
			return Vector4F(b.x*a.m00 + b.y*a.m10 + b.z*a.m20 + b.w*a.m30,
							b.x*a.m01 + b.y*a.m11 + b.z*a.m21 + b.w*a.m31,
							b.x*a.m02 + b.y*a.m12 + b.z*a.m22 + b.w*a.m32,
							b.x*a.m03 + b.y*a.m13 + b.z*a.m23 + b.w*a.m33);
		}

	};
};
