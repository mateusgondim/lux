#include "core/transform.h"

#include <cmath>

#include "core/vec3.h"
#include "core/mat4.h"
#include "core/math.h"

namespace lux {
  Transform::Transform(const Mat4 & mat4, const Mat4 & mat4_inv)
      : m_mat4(mat4), m_mat4_inv(mat4_inv) {}

  Transform::Transform(const Mat4 & mat4) : m_mat4(mat4), m_mat4_inv(inverse(mat4)) {}

  Transform rotate_x(const float ktheta)
  {
    const float kcos_theta = cos(degrees_to_radians(ktheta));
    const float ksin_theta = sin(degrees_to_radians(ktheta));

    Mat4 r(1.0f, 0.0f, 0.0f, 0.0f,
           0.0f, kcos_theta, ksin_theta, 0.0f,
           0.0f, -ksin_theta, kcos_theta, 0.0f,
           0.0f, 0.0f, 0.0f, 1.0f);

    Mat4 r_inv(1.0f, 0.0f, 0.0f, 0.0f,
               0.0f, kcos_theta, -ksin_theta, 0.0f,
               0.0f, ksin_theta, kcos_theta, 0.0f,
               0.0f, 0.0f, 0.0f, 1.0f);

    return Transform(r, r_inv);
  }

  Transform rotate_y(const float ktheta)
  {
    const float kcos_theta = cos(degrees_to_radians(ktheta));
    const float ksin_theta = sin(degrees_to_radians(ktheta));

    Mat4 r(kcos_theta, 0.0f, -ksin_theta, 0.0f,
           0.0f, 1.0f, 0.0f, 0.0f,
           ksin_theta, 0.0f, kcos_theta, 0.0f,
           0.0f, 0.0f, 0.0f, 1.0f);

    Mat4 r_inv(kcos_theta, 0.0f, ksin_theta, 0.0f,
               0.0f, 1.0f, 0.0f, 0.0f,
               -ksin_theta, 0.0f, kcos_theta, 0.0f,
               0.0f, 0.0f, 0.0f, 1.0f);

    return Transform(r, r_inv);
  }

  Transform rotate_z(const float ktheta)
  {
    const float kcos_theta = cos(degrees_to_radians(ktheta));
    const float ksin_theta = sin(degrees_to_radians(ktheta));

    Mat4 r(kcos_theta, ksin_theta, 0.0f, 0.0f,
           -ksin_theta, kcos_theta, 0.0f, 0.0f,
           0.0f, 0.0f, 1.0f, 0.0f,
           0.0f, 0.0f, 0.0f, 1.0f);

    Mat4 r_inv(kcos_theta, -ksin_theta, 0.0f, 0.0f,
               ksin_theta, kcos_theta, 0.0f, 0.0f,
               0.0f, 0.0f, 1.0f, 0.0f,
               0.0f, 0.0f, 0.0f, 1.0f);

    return Transform(r, r_inv);
  }

  Transform scale(const float kx, const float ky, const float kz)
  {
    Mat4 s(kx, 0.0f, 0.0f, 0.0f,
           0.0f, ky, 0.0f, 0.0f,
           0.0f, 0.0f, kz, 0.0f,
           0.0f, 0.0f, 0.0f, 1.0f);

    Mat4 s_inv(1.0f/kx, 0.0f, 0.0f, 0.0f,
               0.0f, 1.0f/ky, 0.0f, 0.0f,
               0.0f, 0.0f, 1.0f/kz, 0.0f,
               0.0f, 0.0f, 0.0f, 1.0f);

    return Transform(s, s_inv);
  }

  Transform translate(const Vec3 & kdelta)
  {
    Mat4 t(1.0f, 0.0f, 0.0f, 0.0f,
           0.0f, 1.0f, 0.0f, 0.0f,
           0.0f, 0.0f, 1.0f, 0.0f,
           kdelta.x, kdelta.y, kdelta.z, 1.0f);

    Mat4 t_inv(1.0f, 0.0f, 0.0f, 0.0f,
               0.0f, 1.0f, 0.0f, 0.0f,
               0.0f, 0.0f, 1.0f, 0.0f,
               -kdelta.x, -kdelta.y, -kdelta.z, 1.0f);

    return Transform(t, t_inv);
  }

  Transform look_at(const Vec3 & eye, const Vec3 & look, const Vec3 & up)
  {
    Vec3 r = normalize(look - eye);
    Vec3 p = normalize(cross(up, r));
    Vec3 q = cross(r, p);

    Mat4 camera_to_world(p.x, p.y, p.z, 0.0f,
                         q.x, q.y, q.z, 0.0f,
                         r.x, r.y, r.z, 0.0f,
                         eye.x, eye.y, eye.z, 1.0f);

    return Transform(camera_to_world, inverse(camera_to_world));
  }
}
