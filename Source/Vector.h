#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>
#include <cfloat>

class QAngle;
class Vector;

typedef float vec_t;

class Vector2D
{
public:
	inline Vector2D(void) : x(0.0f), y(0.0f) {}
	inline Vector2D(float X, float Y) : x(X), y(Y) {}
	inline Vector2D operator+(const Vector2D& v)	const { return Vector2D(x + v.x, y + v.y); }
	inline Vector2D operator-(const Vector2D& v)	const { return Vector2D(x - v.x, y - v.y); }
	inline Vector2D operator*(float fl)				const { return Vector2D(x * fl, y * fl); }
	inline Vector2D operator/(float fl)				const { return Vector2D(x / fl, y / fl); }

	inline float Length(void)						const { return (float)std::sqrt(x * x + y * y); }

	inline Vector2D Normalize(void) const
	{
		float flLen = Length();
		if (flLen == 0.0f)
			return Vector2D(0.0f, 0.0f);
		flLen = 1.0f / flLen;
		return Vector2D(x * flLen, y * flLen);
	}

	inline static float DotProduct(const Vector2D& a, const Vector2D& b) { return (a.x * b.x + a.y * b.y); }

	vec_t	x, y;
};

class Vector
{
public:
	// constructors / dtor
	inline Vector(void) : x(0.0f), y(0.0f), z(0.0f) {}
	inline Vector(float X, float Y, float Z) : x(X), y(Y), z(Z) {}
	inline Vector(float* p) { *this = p; }
	inline Vector(const Vector& in) { x = in.x; y = in.y; z = in.z; }
	inline ~Vector() {}

	// assignment ops
	inline Vector& operator= (const Vector& in);
	inline Vector& operator= (float* p);
	inline Vector& operator= (float f);

	// operator[] : provide both const and non-const overloads
	inline float& operator[] (int i) { return ((float*)this)[i]; }
	inline float  operator[] (int i) const { return ((const float*)this)[i]; }

	inline bool operator! (void) const;

	inline bool operator== (const Vector& other) const;
	inline bool operator!= (const Vector& other) const;

	// arithmetic
	inline Vector& operator+= (const Vector& other);
	inline Vector& operator+= (float* p);
	inline Vector& operator+= (float f);

	inline Vector& operator-= (const Vector& other);
	inline Vector& operator-= (float* p);
	inline Vector& operator-= (float f);

	inline Vector& operator*= (const Vector& other);
	inline Vector& operator*= (float* p);
	inline Vector& operator*= (float f);

	inline Vector& operator/= (const Vector& other);
	inline Vector& operator/= (float* p);
	inline Vector& operator/= (float f);

	inline Vector operator+ (const Vector& other) const;
	inline Vector operator+ (float* p) const;
	inline Vector operator+ (float f) const;

	inline Vector operator- (const Vector& other) const;
	inline Vector operator- (float* p) const;
	inline Vector operator- (float f) const;
	inline Vector operator- (void) const;

	inline Vector operator* (const Vector& other) const;
	inline Vector operator* (float* p) const;
	inline Vector operator* (float f) const;

	inline Vector operator/ (const Vector& other) const;
	inline Vector operator/ (float* p) const;
	inline Vector operator/ (float f) const;

	// conversions
	operator float* () { return &x; }
	operator const float* () const { return &x; }

	// test helpers
	inline bool IsZero(void) const { return x == 0.0f && y == 0.0f && z == 0.0f; }
	inline bool IsZero2D(void) const { return x == 0.0f && y == 0.0f; }

	inline Vector& Clear(void) { x = y = z = 0.0f; return *this; }

	inline Vector& Init(float X, float Y, float Z) { x = X; y = Y; z = Z; return *this; }
	inline Vector& Init(float* p) { *this = p; return *this; }

	inline Vector& Negate(void) { x = -x; y = -y; z = -z; return *this; }

	// dot products: add overload for float*
	inline float Dot(const Vector& other) const { return x * other.x + y * other.y + z * other.z; }
	inline float Dot2D(const Vector& other) const { return x * other.x + y * other.y; }
	inline float Dot(const float* p) const { return x * p[0] + y * p[1] + z * p[2]; } // <-- NEW overload

	// lengths
	//inline float Length(void) const { return std::sqrt(LengthSqr()); }
	inline float Length(void)
	{
		float Length = 0;

		Length = sqrt(LengthSqr());

		return Length;
	}
	inline float Length2D(void) const { return std::sqrt(LengthSqr2D()); }
	inline float LengthSqr(void) const { return x * x + y * y + z * z; }
	inline float LengthSqr2D(void) const { return x * x + y * y; }

	inline float Distance(Vector& ToVector) const { return (ToVector - *this).Length(); }
	inline float Distance2D(Vector& ToVector) const { return (ToVector - *this).Length2D(); }

	// normalize: same semantics as your file2
	inline float Normalize(void)
	{
		float VectorLength = Length();
		if (VectorLength != 0.0f)
		{
			float RecipLength = 1.0f / VectorLength;
			x *= RecipLength; y *= RecipLength; z *= RecipLength;
		}
		return VectorLength;
	}

	inline Vector Normalized(void)
	{
		float flLen = Length();
		if (flLen == 0.0f) return Vector(0, 0, 1);
		flLen = 1.0f / flLen;
		return Vector(x * flLen, y * flLen, z * flLen);
	}

	// 2D normalize semantics preserved
	inline float Normalize2D(void)
	{
		float len = Length2D();
		if (len != 0.0f)
		{
			float RecipLength = 1.0f / len;
			x *= RecipLength; y *= RecipLength;
		}
		return len;
	}

	inline float AngleBetween(Vector& other)
	{
		other.Normalize();
		Normalize();
		float d = Dot(other);
		if (d > 1.0f) d = 1.0f; if (d < -1.0f) d = -1.0f;
		return (float)(acos(d) * 180.0 / M_PI);
	}

	// cross product that fills this vector
	Vector& CrossProduct(const Vector& a, const Vector& b)
	{
		x = (a.y * b.z) - (a.z * b.y);
		y = (a.z * b.x) - (a.x * b.z);
		z = (a.x * b.y) - (a.y * b.x);
		return *this;
	}

	// the following are declared; definitions provided in Vector.cpp
	QAngle ToEulerAngles();
	QAngle ToEulerAngles(Vector* PseudoUp);
	void AngleMatrix(QAngle& Rotation, float (*matrix)[3]);
	void VectorRotate(Vector& In, QAngle& Rotation);

	inline void VectorVectors(Vector& Right, Vector& Up)
	{
		Vector tmp;
		if (x == 0.0f && y == 0.0f)
		{
			Right[0] = 1; Right[1] = 0; Right[2] = 0;
			Up[0] = -z; Up[1] = 0; Up[2] = 0;
			return;
		}
		tmp[0] = 0; tmp[1] = 0; tmp[2] = 1.0f;
		CrossProduct(tmp, Right);
		Right.Normalize();
		Right.CrossProduct(*this, Up);
		Up.Normalize();
	}

	inline bool IsValid() const { return std::isfinite(x) && std::isfinite(y) && std::isfinite(z); }

public:
	float x, y, z;
};

// inline implementations for many ops (kept same semantics as original)
inline Vector& Vector::operator= (const Vector& in) { x = in.x; y = in.y; z = in.z; return *this; }
inline Vector& Vector::operator= (float* p) { if (p) { x = p[0]; y = p[1]; z = p[2]; } else x = y = z = 0; return *this; }
inline Vector& Vector::operator= (float f) { x = y = z = f; return *this; }
inline bool Vector::operator! (void) const { return IsZero(); }
inline bool Vector::operator== (const Vector& other) const { return x == other.x && y == other.y && z == other.z; }
inline bool Vector::operator!= (const Vector& other) const { return !(*this == other); }

inline Vector& Vector::operator+= (const Vector& other) { x += other.x; y += other.y; z += other.z; return *this; }
inline Vector& Vector::operator+= (float* p) { x += p[0]; y += p[1]; z += p[2]; return *this; }
inline Vector& Vector::operator+= (float f) { x += f; y += f; z += f; return *this; }

inline Vector& Vector::operator-= (const Vector& other) { x -= other.x; y -= other.y; z -= other.z; return *this; }
inline Vector& Vector::operator-= (float* p) { x -= p[0]; y -= p[1]; z -= p[2]; return *this; }
inline Vector& Vector::operator-= (float f) { x -= f; y -= f; z -= f; return *this; }

inline Vector& Vector::operator*= (const Vector& other) { x *= other.x; y *= other.y; z *= other.z; return *this; }
inline Vector& Vector::operator*= (float* p) { x *= p[0]; y *= p[1]; z *= p[2]; return *this; }
inline Vector& Vector::operator*= (float f) { x *= f; y *= f; z *= f; return *this; }

inline Vector& Vector::operator/= (const Vector& other)
{
	if (other.x != 0 && other.y != 0 && other.z != 0) { x /= other.x; y /= other.y; z /= other.z; }
	return *this;
}
inline Vector& Vector::operator/= (float* p)
{
	if (p[0] != 0 && p[1] != 0 && p[2] != 0) { x /= p[0]; y /= p[1]; z /= p[2]; }
	return *this;
}
inline Vector& Vector::operator/= (float f) { if (f != 0) { x /= f; y /= f; z /= f; } return *this; }

inline Vector Vector::operator+ (const Vector& other) const { return Vector(x + other.x, y + other.y, z + other.z); }
inline Vector Vector::operator+ (float* p) const { return Vector(x + p[0], y + p[1], z + p[2]); }
inline Vector Vector::operator+ (float f) const { return Vector(x + f, y + f, z + f); }

inline Vector  Vector::operator- (const Vector& other) const { return Vector(x - other.x, y - other.y, z - other.z); }
inline Vector  Vector::operator- (float* p) const { return Vector(x - p[0], y - p[1], z - p[2]); }
inline Vector  Vector::operator- (float f) const { return Vector(x - f, y - f, z - f); }

inline Vector Vector::operator- (void) const { return Vector(-x, -y, -z); }

inline Vector Vector::operator* (const Vector& other) const { return Vector(x * other.x, y * other.y, z * other.z); }
inline Vector Vector::operator* (float* p) const { return Vector(x * p[0], y * p[1], z * p[2]); }
inline Vector Vector::operator* (float f) const { return Vector(x * f, y * f, z * f); }

inline Vector Vector::operator/ (const Vector& other) const { if (other.x != 0 && other.y != 0 && other.z != 0) return Vector(x / other.x, y / other.y, z / other.z); return *this; }
inline Vector Vector::operator/ (float* p) const { if (p[0] != 0 && p[1] != 0 && p[2] != 0) return Vector(x / p[0], y / p[1], z / p[2]); return *this; }
inline Vector Vector::operator/ (float f) const { if (f != 0) return Vector(x / f, y / f, z / f); return *this; }

inline Vector operator*(float fl, const Vector& v) { return v * fl; };
inline float DotProduct(const Vector& a, const Vector& b) { return(a.x * b.x + a.y * b.y + a.z * b.z); }
#define vec3_t Vector
