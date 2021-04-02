#ifndef LUX_CORE_MAT4_H_
#define LUX_CORE_MAT4_H_

#include <iostream>
#include <iomanip>

#include "core/error.h"

namespace lux { class Transform; }

namespace lux {
  class Mat4 final {
    friend Transform;
    friend std::ostream & operator<<(std::ostream & os, const Mat4 & m);
    friend Mat4 transpose(const Mat4 & m);
    friend Mat4 inverse(const Mat4 & m);
    public:
      Mat4() {
        m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
        m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = 0.0f;
        m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = 0.0f;
        m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
      }

      Mat4(const float m00, const float m01, const float m02, const float m03,
           const float m10, const float m11, const float m12, const float m13,
           const float m20, const float m21, const float m22, const float m23,
           const float m30, const float m31, const float m32, const float m33)
      {
        m[0][0] = m00; m[0][1] = m01; m[0][2] = m02; m[0][3] = m03;
        m[1][0] = m10; m[1][1] = m11; m[1][2] = m12; m[1][3] = m13;
        m[2][0] = m20; m[2][1] = m21; m[2][2] = m22; m[2][3] = m23;
        m[3][0] = m30; m[3][1] = m31; m[3][2] = m32; m[3][3] = m33;
      }

      Mat4(const Mat4 & mat4) = default;
      ~Mat4() = default;

      float determinant() const;

      Mat4 & operator=(const Mat4 & rhs) = default;
      Mat4 & operator*=(const Mat4 & rhs);
      Mat4 & operator*=(const float k);
    private:
      float m[4][4];
  };

  inline Mat4 & Mat4::operator*=(const Mat4 & rhs)
  {
    float row[4];
    for (unsigned r = 0; r != 4; ++r) {
      row[0] = m[r][0]; row[1] = m[r][1]; row[2] = m[r][2]; row[3] = m[r][3];
      for (unsigned c = 0; c != 4; ++c) {
        m[r][c] = row[0] * rhs.m[0][c] + row[1] * rhs.m[1][c] + row[2] * rhs.m[2][c] +
                  row[3] * rhs.m[3][c];
      }
    }

    return *this;
  }

  inline Mat4 operator*(const Mat4 & lhs, const Mat4 & rhs)
  {
    Mat4 r(lhs);
    r *= rhs;

    return r;
  }

  inline Mat4 & Mat4::operator*=(const float k)
  {
    m[0][0] *= k; m[0][1] *= k; m[0][2] *= k; m[0][3] *= k;
    m[1][0] *= k; m[1][1] *= k; m[1][2] *= k; m[1][3] *= k;
    m[2][0] *= k; m[2][1] *= k; m[2][2] *= k; m[2][3] *= k;
    m[3][0] *= k; m[3][1] *= k; m[3][2] *= k; m[3][3] *= k;

    return *this;
  }

  inline Mat4 operator*(const float k, const Mat4 & m)
  {
    Mat4 r = m;
    r *= k;

    return r;
  }

  inline Mat4 transpose(const Mat4 & m) {
    return Mat4(m.m[0][0], m.m[1][0], m.m[2][0], m.m[3][0],
                m.m[0][1], m.m[1][1], m.m[2][1], m.m[3][1],
                m.m[0][2], m.m[1][2], m.m[2][2], m.m[3][2],
                m.m[0][3], m.m[1][3], m.m[2][3], m.m[3][3]);
  }

  inline float determinant3x3(const float m00, const float m01, const float m02,
                              const float m10, const float m11, const float m12,
                              const float m20, const float m21, const float m22)
  {
    return m00*m11*m22 + m01*m12*m20 + m02*m10*m21 - m02*m11*m20 - m01*m10*m22 - m00*m12*m21;
  }

  inline float Mat4::determinant() const
  {
    float det = m[0][0] * determinant3x3(m[1][1], m[1][2], m[1][3],
                                         m[2][1], m[2][2], m[2][3],
                                         m[3][1], m[3][2], m[3][3]);

    det += -m[0][1] * determinant3x3(m[1][0], m[1][2], m[1][3],
                                     m[2][0], m[2][2], m[2][3],
                                     m[3][0], m[3][2], m[3][3]);

    det += m[0][2] * determinant3x3(m[1][0], m[1][1], m[1][3],
                                    m[2][0], m[2][1], m[2][3],
                                    m[3][0], m[3][1], m[3][3]);

    det += -m[0][3] * determinant3x3(m[1][0], m[1][1], m[1][2],
                                     m[2][0], m[2][1], m[2][2],
                                     m[3][0], m[3][1], m[3][2]);

    return det;
  }

  // Calculates the inverse using the Classical Adjoint method
  inline Mat4 inverse(const Mat4 & m)
  {
    const float kdet = m.determinant();

    ASSERT(kdet != 0.0f, "Trying to invert a non invertible matrix!");

    Mat4 inv;

    inv.m[0][0] = determinant3x3(m.m[1][1], m.m[1][2], m.m[1][3],
                                 m.m[2][1], m.m[2][2], m.m[2][3],
                                 m.m[3][1], m.m[3][2], m.m[3][3]);

    inv.m[0][1] = - determinant3x3(m.m[1][0], m.m[1][2], m.m[1][3],
                                   m.m[2][0], m.m[2][2], m.m[2][3],
                                   m.m[3][0], m.m[3][2], m.m[3][3]);

    inv.m[0][2] = determinant3x3(m.m[1][0], m.m[1][1], m.m[1][3],
                                 m.m[2][0], m.m[2][1], m.m[2][3],
                                 m.m[3][0], m.m[3][1], m.m[3][3]);

    inv.m[0][3] = - determinant3x3(m.m[1][0], m.m[1][1], m.m[1][2],
                                   m.m[2][0], m.m[2][1], m.m[2][2],
                                   m.m[3][0], m.m[3][1], m.m[3][2]);

    inv.m[1][0] = - determinant3x3(m.m[0][1], m.m[0][2], m.m[0][3],
                                   m.m[2][1], m.m[2][2], m.m[2][3],
                                   m.m[3][1], m.m[3][2], m.m[3][3]);

    inv.m[1][1] = determinant3x3(m.m[0][0], m.m[0][2], m.m[0][3],
                                 m.m[2][0], m.m[2][2], m.m[2][3],
                                 m.m[3][0], m.m[3][2], m.m[3][3]);

    inv.m[1][2] = - determinant3x3(m.m[0][0], m.m[0][1], m.m[0][3],
                                   m.m[2][0], m.m[2][1], m.m[2][3],
                                   m.m[3][0], m.m[3][1], m.m[3][3]);

    inv.m[1][3] = determinant3x3(m.m[0][0], m.m[0][1], m.m[0][2],
                                 m.m[2][0], m.m[2][1], m.m[2][2],
                                 m.m[3][0], m.m[3][1], m.m[3][2]);

    inv.m[2][0] = determinant3x3(m.m[0][1], m.m[0][2], m.m[0][3],
                                 m.m[1][1], m.m[1][2], m.m[1][3],
                                 m.m[3][1], m.m[3][2], m.m[3][3]);

    inv.m[2][1] = - determinant3x3(m.m[0][0], m.m[0][2], m.m[0][3],
                                   m.m[1][0], m.m[1][2], m.m[1][3],
                                   m.m[3][0], m.m[3][2], m.m[3][3]);

    inv.m[2][2] = determinant3x3(m.m[0][0], m.m[0][1], m.m[0][3],
                                 m.m[1][0], m.m[1][1], m.m[1][3],
                                 m.m[3][0], m.m[3][1], m.m[3][3]);

    inv.m[2][3] = - determinant3x3(m.m[0][0], m.m[0][1], m.m[0][2],
                                   m.m[1][0], m.m[1][1], m.m[1][2],
                                   m.m[3][0], m.m[3][1], m.m[3][2]);

    inv.m[3][0] = - determinant3x3(m.m[0][1], m.m[0][2], m.m[0][3],
                                   m.m[1][1], m.m[1][2], m.m[1][3],
                                   m.m[2][1], m.m[2][2], m.m[2][3]);

    inv.m[3][1] = determinant3x3(m.m[0][0], m.m[0][2], m.m[0][3],
                                 m.m[1][0], m.m[1][2], m.m[1][3],
                                 m.m[2][0], m.m[2][2], m.m[2][3]);

    inv.m[3][2] = - determinant3x3(m.m[0][0], m.m[0][1], m.m[0][3],
                                   m.m[1][0], m.m[1][1], m.m[1][3],
                                   m.m[2][0], m.m[2][1], m.m[2][3]);

    inv.m[3][3] = determinant3x3(m.m[0][0], m.m[0][1], m.m[0][2],
                                 m.m[1][0], m.m[1][1], m.m[1][2],
                                 m.m[2][0], m.m[2][1], m.m[2][2]);

    inv = transpose(inv);

    const float kone_over_det = 1.0f / kdet;
    inv *= kone_over_det;
    
    return inv;
  }

  inline std::ostream & operator<<(std::ostream & os, const Mat4 & m)
  {
    std::streamsize stream_precision = os.precision();
    os.precision(4);
    os << std::fixed;
    os << "|" << std::setw(10) << m.m[0][0] << std::setw(10) << m.m[0][1]
       << std::setw(10) << m.m[0][2] << std::setw(10) << m.m[0][3] << " |" << std::endl;

    os << "|" << std::setw(10) << m.m[1][0] << std::setw(10) << m.m[1][1]
       << std::setw(10) << m.m[1][2] << std::setw(10) << m.m[1][3] << " |" << std::endl;

    os << "|" << std::setw(10) << m.m[2][0] << std::setw(10) << m.m[2][1]
       << std::setw(10) << m.m[2][2] << std::setw(10) << m.m[2][3] << " |" << std::endl;

    os << "|" << std::setw(10) << m.m[3][0] << std::setw(10) << m.m[3][1]
       << std::setw(10) << m.m[3][2] << std::setw(10) << m.m[3][3] << " |" << std::endl;

    os.precision(stream_precision);

    return os;
  }
}
#endif
