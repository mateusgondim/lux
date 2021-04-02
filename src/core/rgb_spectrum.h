#ifndef LUX_CORE_RGB_SPECTRUM_H_
#define LUX_CORE_RGB_SPECTRUM_H_

#include <iostream>

#include "core/error.h"

namespace lux {
  class RGB_spectrum {
    friend bool operator==(const RGB_spectrum & lhs, const RGB_spectrum & rhs);
    public:
      explicit RGB_spectrum(const float v = 0.0f) : m_rgb{ v, v, v } {}
      RGB_spectrum(const float r, const float g, const float b) : m_rgb{ r, g, b } {}

      RGB_spectrum(const RGB_spectrum & spectrum)
        : m_rgb{ spectrum.m_rgb[0], spectrum.m_rgb[1], spectrum.m_rgb[2] } {} 

      RGB_spectrum & operator=(const RGB_spectrum & spectrum)
      {
        m_rgb[0] = spectrum.m_rgb[0];
        m_rgb[1] = spectrum.m_rgb[1];
        m_rgb[2] = spectrum.m_rgb[2];

        return *this;
      }

      RGB_spectrum & operator+=(const RGB_spectrum & spectrum);
      RGB_spectrum & operator-=(const RGB_spectrum & spectrum);
      RGB_spectrum & operator*=(const RGB_spectrum & spectrum);
      RGB_spectrum & operator*=(const float k);
      RGB_spectrum & operator/=(const RGB_spectrum & spectrum);
      RGB_spectrum & operator/=(const float k);
      RGB_spectrum & clamp();

      float x() const { return m_rgb[0]; }
      float y() const { return m_rgb[1]; }
      float z() const { return m_rgb[2]; }
      bool is_black() const;

      float & operator[](const unsigned i);
      const float & operator[](const unsigned i) const;

    private:
      float m_rgb[3];
  };

  inline const float & RGB_spectrum::operator[](const unsigned i) const
  {
    ASSERT(i < 3, "Trying to access a non existent RGB_spectrum component");

    if (i == 0) return m_rgb[0];
    if (i == 1) return m_rgb[1];
    return m_rgb[2];
  }

  inline float & RGB_spectrum::operator[](const unsigned i)
  {
    return const_cast<float &>(static_cast<const RGB_spectrum &>(*this)[i]);
  }

  inline RGB_spectrum & RGB_spectrum::operator+=(const RGB_spectrum & spectrum)
  {
    m_rgb[0] += spectrum.m_rgb[0];
    m_rgb[1] += spectrum.m_rgb[1];
    m_rgb[2] += spectrum.m_rgb[2];

    return *this;
  }

  inline RGB_spectrum operator+(const RGB_spectrum & lhs, const RGB_spectrum & rhs)
  {
    RGB_spectrum r(lhs);
    r += rhs;

    return r;
  }

  inline RGB_spectrum & RGB_spectrum::operator-=(const RGB_spectrum & spectrum)
  {
    m_rgb[0] -= spectrum.m_rgb[0];
    m_rgb[1] -= spectrum.m_rgb[1];
    m_rgb[2] -= spectrum.m_rgb[2];

    return *this;
  }

  inline RGB_spectrum operator-(const RGB_spectrum & lhs, const RGB_spectrum & rhs)
  {
    RGB_spectrum r(lhs);
    r -= rhs;

    return r;
  }

  inline RGB_spectrum & RGB_spectrum::operator*=(const RGB_spectrum & spectrum)
  {
    m_rgb[0] *= spectrum.m_rgb[0];
    m_rgb[1] *= spectrum.m_rgb[1];
    m_rgb[2] *= spectrum.m_rgb[2];

    return *this;
  }

  inline RGB_spectrum operator*(const RGB_spectrum & lhs, const RGB_spectrum & rhs)
  {
    RGB_spectrum r(lhs);
    r *= rhs;

    return r;
  }

  inline RGB_spectrum & RGB_spectrum::operator*=(const float k)
  {
    m_rgb[0] *= k;
    m_rgb[1] *= k;
    m_rgb[2] *= k;

    return *this;
  }

  inline RGB_spectrum operator*(const float k, const RGB_spectrum & spectrum)
  {
    RGB_spectrum r(spectrum);
    r *= k;

    return r;
  }

  inline RGB_spectrum operator*(const RGB_spectrum & spectrum, const float k)
  {
    return k * spectrum;
  }

  inline RGB_spectrum & RGB_spectrum::operator/=(const RGB_spectrum & spectrum)
  {
    m_rgb[0] /= spectrum.m_rgb[0];
    m_rgb[1] /= spectrum.m_rgb[1];
    m_rgb[2] /= spectrum.m_rgb[2];

    return *this;
  }

  inline RGB_spectrum operator/(const RGB_spectrum & lhs, const RGB_spectrum & rhs)
  {
    RGB_spectrum r(lhs);
    r /= rhs;

    return r;
  }

  inline RGB_spectrum & RGB_spectrum::operator/=(const float k)
  {
    m_rgb[0] /= k;
    m_rgb[1] /= k;
    m_rgb[2] /= k;

    return *this;
  }

  inline RGB_spectrum operator/(const RGB_spectrum & spectrum, const float k)
  {
    RGB_spectrum r(spectrum);
    r /= k;

    return r;
  }

  inline RGB_spectrum lerp(const float t, const RGB_spectrum & s0, const RGB_spectrum & s1)
  {
    return (1-t) * s0 + t * s1;
  }

  RGB_spectrum clamp(const RGB_spectrum & spectrum);

  bool operator==(const RGB_spectrum & lhs, const RGB_spectrum & rhs);
  bool operator!=(const RGB_spectrum & lhs, const RGB_spectrum & rhs); 

  std::ostream & operator<<(std::ostream & os, const RGB_spectrum & spectrum);
}
#endif
