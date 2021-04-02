#ifndef LUX_CORE_TRANSFORM_H_
#define LUX_CORE_TRANSFORM_H_

#include <iostream>

#include "core/mat4.h"
#include "core/vec3.h"
#include "core/ray.h"

namespace lux {
  class Transform final {
    friend std::ostream & operator<<(std::ostream & os, const Transform & t);
    friend Transform operator*(const Transform & lhs, const Transform & rhs);
    friend Transform inverse(const Transform & t);
    public:
      Transform() = default;
      Transform(const Mat4 & mat4, const Mat4 & mat4_inv);
      explicit Transform(const Mat4 & mat4);
      Transform(const Transform &) = default;

      ~Transform() = default;

      Vec3 apply_on_vector(const Vec3 & v) const;
      Vec3 apply_inverse_on_vector(const Vec3 & v) const;

      Vec3 apply_on_point(const Vec3 & p) const;
      Vec3 apply_inverse_on_point(const Vec3 & p) const;

      Vec3 apply_on_normal(const Vec3 & n) const;
      Vec3 apply_inverse_on_normal(const Vec3 & n) const;

      Ray  apply_on_ray(const Ray & r) const;
      Ray  apply_inverse_on_ray(const Ray & r) const;

      Transform & operator=(const Transform &) = default;

    private:
      Vec3 apply_on_vector(const Vec3 & v, const Mat4 & m) const;
      Vec3 apply_on_point(const Vec3 & p, const Mat4 & m) const;
      Vec3 apply_on_normal(const Vec3 & n, const Mat4 & m) const;

      Mat4 m_mat4;
      Mat4 m_mat4_inv;
  };


  inline Vec3 Transform::apply_on_vector(const Vec3 & v, const Mat4 & m) const
  {
    float x, y, z;
    x = v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0];
    y = v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1];
    z = v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2];

    return Vec3(x, y, z);
  }

  inline Vec3 Transform::apply_on_point(const Vec3 & p, const Mat4 & m) const
  {
    float x, y, z, w;
    x = p.x * m.m[0][0] + p.y * m.m[1][0] + p.z * m.m[2][0] + m.m[3][0];
    y = p.x * m.m[0][1] + p.y * m.m[1][1] + p.z * m.m[2][1] + m.m[3][1];
    z = p.x * m.m[0][2] + p.y * m.m[1][2] + p.z * m.m[2][2] + m.m[3][2];
    w = p.x * m.m[0][3] + p.y * m.m[1][3] + p.z * m.m[2][3] + m.m[3][3];

    if (w == 1.0f) return Vec3(x, y, z);

    const float kone_over_w = 1.0f / w;
    return kone_over_w * Vec3(x, y, z);
  }

  inline Vec3 Transform::apply_on_normal(const Vec3 & n, const Mat4 & m) const
  {
    return Vec3(n.x * m.m[0][0] + n.y * m.m[0][1] + n.z * m.m[0][2],
                n.x * m.m[1][0] + n.y * m.m[1][1] + n.z * m.m[1][2],
                n.x * m.m[2][0] + n.y * m.m[2][1] + n.z * m.m[2][2]);
  }

  inline Vec3 Transform::apply_on_vector(const Vec3 & v) const
  {
    return apply_on_vector(v, m_mat4);
  }

  inline Vec3 Transform::apply_inverse_on_vector(const Vec3 & v) const
  {
    return apply_on_vector(v, m_mat4_inv);
  }

  inline Vec3 Transform::apply_on_point(const Vec3 & p) const
  {
    return apply_on_point(p, m_mat4);
  }

  inline Vec3 Transform::apply_inverse_on_point(const Vec3 & p) const
  {
    return apply_on_point(p, m_mat4_inv);
  }

  inline Vec3 Transform::apply_on_normal(const Vec3 & n) const 
  {
    return apply_on_normal(n, m_mat4_inv);
  }

  inline Vec3 Transform::apply_inverse_on_normal(const Vec3 & n) const
  {
    return apply_on_normal(n, m_mat4);
  }

  inline Ray Transform::apply_on_ray(const Ray & r) const
  {
    const Vec3 o = r.get_origin();
    const Vec3 d = r.get_direction();

    return Ray(apply_on_point(o, m_mat4), apply_on_vector(d, m_mat4), r.get_t_max(), r.get_time());
  }

  inline Ray Transform::apply_inverse_on_ray(const Ray & r) const
  {
    const Vec3 o = r.get_origin();
    const Vec3 d = r.get_direction();

    return Ray(apply_on_point(o, m_mat4_inv), apply_on_vector(d, m_mat4_inv), r.get_t_max(),
        r.get_time());
  }

  inline Transform operator*(const Transform & lhs, const Transform & rhs)
  {
    return Transform(lhs.m_mat4 * rhs.m_mat4, rhs.m_mat4_inv * lhs.m_mat4_inv);
  }

  inline Transform inverse(const Transform & t)
  {
    return Transform(t.m_mat4_inv, t.m_mat4);
  }

  inline std::ostream & operator<<(std::ostream & os, const Transform & t)
  {
    os << "M = " << std::endl << t.m_mat4 << std::endl;
    os << "MInv = " << std::endl << t.m_mat4_inv;

    return os;
  }

  Transform rotate_x(const float ktheta);
  Transform rotate_y(const float ktheta);
  Transform rotate_z(const float ktheta);
  Transform scale(const float kx, const float ky, const float kz);
  Transform translate(const Vec3 & kdelta);
  Transform look_at(const Vec3 & eye, const Vec3 & look, const Vec3 & up = Vec3(0.0f, 1.0f, 0.0f));
}

#endif

