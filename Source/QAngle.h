#pragma once
#include <cmath>

class Vector; // Forward declare

class QAngle
{
public:
    // Constructor / Destructor
    QAngle() : x(0.f), y(0.f), z(0.f) {}
    QAngle(float X, float Y, float Z) : x(X), y(Y), z(Z) {}
    QAngle(float* p) { *this = p; }
    QAngle(const QAngle& in) { *this = in; }
    ~QAngle() = default;

    // Assignment operators
    inline QAngle& operator= (const QAngle& in);
    inline QAngle& operator= (float* p);
    inline QAngle& operator= (float f);

    // Element access
    inline float& operator[](int i);
    inline const float& operator[](int i) const;

    // Boolean operators
    inline bool operator!() const { return IsZero(); }
    inline bool operator==(const QAngle& other) const { return x == other.x && y == other.y && z == other.z; }
    inline bool operator!=(const QAngle& other) const { return !(*this == other); }

    // Arithmetic operators
    inline QAngle& operator+=(const QAngle& other);
    inline QAngle& operator+=(float* p);
    inline QAngle& operator+=(float f);

    inline QAngle& operator-=(const QAngle& other);
    inline QAngle& operator-=(float* p);
    inline QAngle& operator-=(float f);

    inline QAngle& operator*=(const QAngle& other);
    inline QAngle& operator*=(float* p);
    inline QAngle& operator*=(float f);

    inline QAngle& operator/=(const QAngle& other);
    inline QAngle& operator/=(float* p);
    inline QAngle& operator/=(float f);

    inline QAngle operator+(const QAngle& other) const;
    inline QAngle operator+(float* p) const;
    inline QAngle operator+(float f) const;

    inline QAngle operator-(const QAngle& other) const;
    inline QAngle operator-(float* p) const;
    inline QAngle operator-(float f) const;
    inline QAngle operator-() const;

    inline QAngle operator*(const QAngle& other) const;
    inline QAngle operator*(float* p) const;
    inline QAngle operator*(float f) const;

    inline QAngle operator/(const QAngle& other) const;
    inline QAngle operator/(float* p) const;
    inline QAngle operator/(float f) const;

    // Conversion operators
    operator float* () { return &x; }
    operator const float* () const { return &x; }

    // Utility
    inline bool IsZero() const { return x == 0.f && y == 0.f && z == 0.f; }
    inline bool IsZero2D() const { return x == 0.f && y == 0.f; }
    inline QAngle& Clear() { x = y = z = 0.f; return *this; }
    inline QAngle& Init(float X, float Y, float Z) { x = X; y = Y; z = Z; return *this; }
    inline QAngle& Init(float* p) { *this = p; return *this; }
    inline QAngle& Negate() { x = -x; y = -y; z = -z; return *this; }

    QAngle& Normalize()
    {
        for (int i = 0; i < 3; i++)
        {
            float* ptr = &x + i;
            if (*ptr > 180.f) *ptr -= 360.f;
            else if (*ptr < -180.f) *ptr += 360.f;
        }
        return *this;
    }

    QAngle Delta360(const QAngle& other) const
    {
        QAngle diff = other - *this;
        for (int i = 0; i < 3; i++)
        {
            if (diff[i] < 0.f)
            {
                diff[i] = -diff[i];
                if (diff[i] > 180.f) diff[i] = 360.f - diff[i];
            }
        }
        return diff;
    }

    bool IsWithinFOV(const QAngle& AngIn, float fovX, float fovY) const
    {
        QAngle diff = Delta360(AngIn);
        return diff.x <= fovX && diff.y <= fovY;
    }

    void AngleVectors(Vector* Forward, Vector* Right, Vector* Up);
    void AngleVectorsTranspose(Vector* Forward, Vector* Right, Vector* Up);

    bool IsValid() const { return std::isfinite(x) && std::isfinite(y) && std::isfinite(z); }

public:
    float x, y, z;
};

// ------------------- Operator Implementations -------------------

inline QAngle& QAngle::operator=(const QAngle& in) { x = in.x; y = in.y; z = in.z; return *this; }
inline QAngle& QAngle::operator=(float* p) { if (p) { x = p[0]; y = p[1]; z = p[2]; } else { x = y = z = 0; } return *this; }
inline QAngle& QAngle::operator=(float f) { x = y = z = f; return *this; }

inline float& QAngle::operator[](int i) { return *((&x) + i); }
inline const float& QAngle::operator[](int i) const { return *((&x) + i); }

inline QAngle& QAngle::operator+=(const QAngle& other) { x += other.x; y += other.y; z += other.z; return *this; }
inline QAngle& QAngle::operator+=(float* p) { x += p[0]; y += p[1]; z += p[2]; return *this; }
inline QAngle& QAngle::operator+=(float f) { x += f; y += f; z += f; return *this; }

inline QAngle& QAngle::operator-=(const QAngle& other) { x -= other.x; y -= other.y; z -= other.z; return *this; }
inline QAngle& QAngle::operator-=(float* p) { x -= p[0]; y -= p[1]; z -= p[2]; return *this; }
inline QAngle& QAngle::operator-=(float f) { x -= f; y -= f; z -= f; return *this; }

inline QAngle& QAngle::operator*=(const QAngle& other) { x *= other.x; y *= other.y; z *= other.z; return *this; }
inline QAngle& QAngle::operator*=(float* p) { x *= p[0]; y *= p[1]; z *= p[2]; return *this; }
inline QAngle& QAngle::operator*=(float f) { x *= f; y *= f; z *= f; return *this; }

inline QAngle& QAngle::operator/=(const QAngle& other) { if (other.x != 0 && other.y != 0 && other.z != 0) { x /= other.x;y /= other.y;z /= other.z; } return *this; }
inline QAngle& QAngle::operator/=(float* p) { if (p[0] != 0 && p[1] != 0 && p[2] != 0) { x /= p[0];y /= p[1];z /= p[2]; } return *this; }
inline QAngle& QAngle::operator/=(float f) { if (f != 0) { x /= f;y /= f;z /= f; } return *this; }

inline QAngle QAngle::operator+(const QAngle& other) const { return QAngle(x + other.x, y + other.y, z + other.z); }
inline QAngle QAngle::operator+(float* p) const { return QAngle(x + p[0], y + p[1], z + p[2]); }
inline QAngle QAngle::operator+(float f) const { return QAngle(x + f, y + f, z + f); }

inline QAngle QAngle::operator-(const QAngle& other) const { return QAngle(x - other.x, y - other.y, z - other.z); }
inline QAngle QAngle::operator-(float* p) const { return QAngle(x - p[0], y - p[1], z - p[2]); }
inline QAngle QAngle::operator-(float f) const { return QAngle(x - f, y - f, z - f); }
inline QAngle QAngle::operator-() const { return QAngle(-x, -y, -z); }

inline QAngle QAngle::operator*(const QAngle& other) const { return QAngle(x * other.x, y * other.y, z * other.z); }
inline QAngle QAngle::operator*(float* p) const { return QAngle(x * p[0], y * p[1], z * p[2]); }
inline QAngle QAngle::operator*(float f) const { return QAngle(x * f, y * f, z * f); }

inline QAngle QAngle::operator/(const QAngle& other) const { if (other.x != 0 && other.y != 0 && other.z != 0) return QAngle(x / other.x, y / other.y, z / other.z); return QAngle(0, 0, 0); }
inline QAngle QAngle::operator/(float* p) const { if (p[0] != 0 && p[1] != 0 && p[2] != 0) return QAngle(x / p[0], y / p[1], z / p[2]); return QAngle(0, 0, 0); }
inline QAngle QAngle::operator/(float f) const { if (f != 0) return QAngle(x / f, y / f, z / f); return QAngle(0, 0, 0); }

inline QAngle operator*(float fl, const QAngle& v) { return v * fl; }
