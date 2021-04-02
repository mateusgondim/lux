#ifndef LUX_CORE_VEC2_H_
#define LUX_CORE_VEC2_H_
#include <cmath>

#include <iostream>

#include "core/error.h"

namespace lux {
  struct Vec2 final {
    Vec2() : x(0.0f), y(0.0f) {}
    Vec2(const float x, const float y) : x(x), y(y) {}
    Vec2(const Vec2 & vec2) : x(vec2.x), y(vec2.y) {}
    ~Vec2() = default;

    const float & operator[](const unsigned i) const;
    float & operator[](const unsigned i);

    Vec2 & operator=(const Vec2 &)  &;
    Vec2 & operator+=(const Vec2 &) &;
    Vec2 & operator-=(const Vec2 &) &;
    Vec2 & operator*=(const float)  &;
    Vec2 & operator/=(const float)  &;
    Vec2 & normalize();

    float magnitude() const;

    float x;
    float y;
  };

  inline const float & Vec2::operator[](const unsigned i) const
  {
    ASSERT(i < 2, "Trying to access a non existent vector element");

    return (i == 0) ? (x) : (y);
  }

  inline float & Vec2::operator[](const unsigned i)
  {
    return const_cast<float &>((static_cast<const Vec2 &>(*this)[i]));
  }

  inline Vec2 & Vec2::operator=(const Vec2 & rhs) &
  {
    x = rhs.x;
    y = rhs.y;

    return *this;
  }

  inline Vec2 & Vec2::operator+=(const Vec2 & rhs) &
  {
    x += rhs.x;
    y += rhs.y;

    return *this;
  }

  inline Vec2 operator+(const Vec2 & lhs, const Vec2 & rhs)
  {
    Vec2 r(lhs);
    r += rhs;

    return r;
  }

  inline Vec2 & Vec2::operator-=(const Vec2 & rhs) &
  {
    x -= rhs.x;
    y -= rhs.y;

    return *this;
  }

  inline Vec2 operator-(const Vec2 & lhs, const Vec2 & rhs)
  {
    Vec2 r(lhs);
    r -= rhs;

    return r;
  }

  inline Vec2 & Vec2::operator*=(const float k) &
  {
    x *= k;
    y *= k;

    return *this;
  }

  inline Vec2 operator*(const Vec2 & lhs, const float k)
  {
    Vec2 r(lhs);
    r *= k;

    return r;
  }

  inline Vec2 operator*(const float k, const Vec2 & rhs )
  {
    Vec2 r(rhs);
    r *= k;

    return r;
  }

  inline Vec2 & Vec2::operator/=(const float k) &
  {
    ASSERT(k != 0.0f, "Division by zero!");

    x /= k;
    y /= k;

    return *this;
  }

  inline Vec2 operator/(const Vec2 & lhs, const float k)
  {
    Vec2 r(lhs);
    r /= k;

    return r;
  }

  inline float Vec2::magnitude() const
  {
    return sqrt(x*x + y*y);
  }

  inline Vec2 & Vec2::normalize() {
    const float length = magnitude();
    operator/=(length);

    return *this;
  }

  inline Vec2 normalize(const Vec2 & v)
  {
    Vec2 r(v);
    r.normalize();

    return r;
  }

  inline float dot(const Vec2 & lhs, const Vec2 & rhs)
  {
    return lhs.x*rhs.x + lhs.y*rhs.y;
  }

  std::ostream & operator<<(std::ostream & os, const Vec2 & v);
}

#endif
