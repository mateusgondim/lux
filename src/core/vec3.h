#ifndef LUX_CORE_VEC3_H_
#define LUX_CORE_VEC3_H_
#include <cmath>

#include <iostream>

#include "core/error.h"

namespace lux {
  struct Vec3 final {
    Vec3() : x(0.0f), y(0.0f), z(0.0f) {}
    Vec3(const float x, const float y, const float z) : x(x), y(y), z(z) {}
    Vec3(const Vec3 & vec3) : x(vec3.x), y(vec3.y), z(vec3.z) {}
    ~Vec3() = default;

    const float & operator[](const unsigned i) const;
    float & operator[](const unsigned i);

    Vec3 & operator=(const Vec3  &) &; 
    Vec3 & operator+=(const Vec3 &) &;
    Vec3 & operator-=(const Vec3 &) &;
    Vec3 & operator*=(const float)  &;
    Vec3 & operator/=(const float)  &;
    Vec3 & normalize();

    float magnitude() const;
    float magnitude_squared() const;

    float x;
    float y;
    float z;
  };

  inline const float & Vec3::operator[](const unsigned i) const
  {
    ASSERT(i < 3, "Trying to access a non existent vector element");

    if (i == 0) return x;
    if (i == 1) return y;
    return z;
  }

  inline float & Vec3::operator[](const unsigned i)
  {
    return const_cast<float &>(static_cast<const Vec3 &>(*this)[i]);
  }

  inline Vec3 & Vec3::operator=(const Vec3 & rhs) &
  {
    x = rhs.x;
    y = rhs.y;
    z = rhs.z;

    return *this;
  }

  inline Vec3 & Vec3::operator+=(const Vec3 & rhs) &
  {
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;

    return * this;
  }

  inline Vec3 operator+(const Vec3 & lhs, const Vec3 & rhs)
  {
    Vec3 r(lhs);
    r += rhs;

    return r;
  }

  inline Vec3 & Vec3::operator-=(const Vec3 & rhs) &
  {
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;

    return * this;
  }

  inline Vec3 operator-(const Vec3 & lhs, const Vec3 & rhs)
  {
    Vec3 r(lhs);
    r -= rhs;

    return r;
  }

  inline Vec3 & Vec3::operator*=(const float k) &
  {
    x *= k;
    y *= k;
    z *= k;

    return * this;
  }

  inline Vec3 operator*(const float k, const Vec3 & v)
  {
    Vec3 r(v);
    r *= k;

    return r;
  }

  inline Vec3 operator*(const Vec3 & v, const float k)
  {
    Vec3 r(v);
    r *= k;

    return r;
  }

  inline Vec3 operator-(const Vec3 & v)
  {
    return Vec3(-v.x, -v.y, -v.z);
  }

  inline Vec3 & Vec3::operator/=(const float k) &
  {
    ASSERT(k != 0.0f, "Division by zero!");

    x /= k;
    y /= k;
    z /= k;

    return * this;
  }

  inline Vec3 operator/(const Vec3 & v, const float k)
  {
    Vec3 r(v);
    r /= k;

    return r;
  }

  inline float dot(const Vec3 & lhs, const Vec3 & rhs)
  {
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
  }

  inline float Vec3::magnitude() const
  {
    return sqrt(x * x + y * y + z * z);
  }

  inline float Vec3::magnitude_squared() const
  {
    return x * x + y * y + z * z;
  }

  inline Vec3 & Vec3::normalize()
  {
    const float mag = magnitude();
    return operator/=(mag);
  }

  inline Vec3 normalize(const Vec3 & v)
  {
    Vec3 r(v);
    r.normalize();

    return r;
  }

  inline Vec3 cross(const Vec3 & a, const Vec3 & b)
  {
    return Vec3(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
  }

  inline float abs_dot(const Vec3 & a, const Vec3 & b)
  {
    return std::abs(dot(a, b));
  }

  inline float magnitude(const Vec3 & v)
  {
    return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
  }

  inline float magnitude_squared(const Vec3 & v)
  {
    return v.x*v.x + v.y*v.y + v.z*v.z;
  }

  inline float distance(const Vec3 & a, const Vec3 & b)
  {
    return magnitude(b - a);
  }

  inline float distance_squared(const Vec3 & a, const Vec3 & b)
  {
    return magnitude_squared(b - a);
  }

  // Expects a NORMALIZED r
  inline void orthonormal_basis(Vec3 * p, Vec3 * q, const Vec3 & r)
  {
    Vec3 a(cross(r, Vec3(1.0f, 0.0f, 0.0f)));
    if (a.magnitude_squared() < 0.01f) {
      *p = normalize(cross(Vec3(0.0f, 1.0f, 0.0f), r));
      *q = cross(r, *p);
    }
    else {
      *q = normalize(a);
      *p = cross(*q, r);
    }
  }

  std::ostream & operator<<(std::ostream & os, const Vec3 & v);
}
#endif
