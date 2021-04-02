#include "shapes/triangle.h"

#include <cmath>

#include "core/ray.h"
#include "core/vec3.h"
#include "core/rgb_spectrum.h"

//TODO: Implement sampling and PDF
namespace lux {
  bool Triangle::intersect(const Ray & ray, float *phit,
                           Surface_interaction *psurface_interaction) const
  {
    // Transform triangle vertices to World Space
    const Transform & object_to_world = get_object_to_world();
    const Vec3 v1_wld = object_to_world.apply_on_point(m_v1);
    const Vec3 v2_wld = object_to_world.apply_on_point(m_v2);
    const Vec3 v3_wld = object_to_world.apply_on_point(m_v3);

    // Compute the plane/triangle normal
    Vec3 normal(cross(v2_wld - v1_wld, v3_wld - v2_wld));
    normal.normalize();

    // Calculate plane distance d
    const float kdistance = dot(v1_wld, normal);

    // Check if ray intersects the plane
    const float kdir_dot_normal = dot(ray.get_direction(), normal);
    if (kdir_dot_normal >= 0.0f) { // ray is parallel or intercepts from the plane back
      return false;
    }

    const float korigin_dot_normal = dot(ray.get_origin(), normal);
    if (korigin_dot_normal == kdistance) { // the ray's origin is on the plane
      return false;
    }

    // Calculate rays's parameter value at the intersection point with the plane
    const float kt = (kdistance - korigin_dot_normal) / kdir_dot_normal;
    if (kt <= 0.0f) {
      return false;
    }

    if (kt > ray.get_t_max()) {
      return false;
    }

    // Compute the 3D point of intersection
    const Vec3 p(ray(kt));

    // Find dominant axis to select plane of projection
    float u1, u2, u3;
    float v1, v2, v3;
    if (fabs(normal.x) > fabs(normal.y)) {
      if (fabs(normal.x) > fabs(normal.z)) {
        u1 = p.y - v1_wld.y;
        u2 = v2_wld.y - v1_wld.y;
        u3 = v3_wld.y - v1_wld.y;

        v1 = p.z - v1_wld.z;
        v2 = v2_wld.z - v1_wld.z;
        v3 = v3_wld.z - v1_wld.z;
      }
      else {
        u1 = p.x - v1_wld.x;
        u2 = v2_wld.x - v1_wld.x;
        u3 = v3_wld.x - v1_wld.x;

        v1 = p.y - v1_wld.y;
        v2 = v2_wld.y - v1_wld.y;
        v3 = v3_wld.y - v1_wld.y;
      }
    }
    else {
      if (fabs(normal.y) > fabs(normal.z)) {
        u1 = p.x - v1_wld.x;
        u2 = v2_wld.x - v1_wld.x;
        u3 = v3_wld.x - v1_wld.x;

        v1 = p.z - v1_wld.z;
        v2 = v2_wld.z - v1_wld.z;
        v3 = v3_wld.z - v1_wld.z;
      }
      else {
        u1 = p.x - v1_wld.x;
        u2 = v2_wld.x - v1_wld.x;
        u3 = v3_wld.x - v1_wld.x;

        v1 = p.y - v1_wld.y;
        v2 = v2_wld.y - v1_wld.y;
        v3 = v3_wld.y - v1_wld.y;
      }
    }

    // Compute denominator
    float temp = u2 * v3 - v2 * u3;
    if (temp == 0.0f) {
      return false;
    }
    temp = 1.0f / temp;

    // Compute Barycentric coordinates, checking for out-of-range.
    const float alpha = (u1 * v3 - v1 * u3) * temp;
    if (alpha < 0.0f) {
      return false;
    }

    const float beta = (u2 * v1 - v2 * u1) * temp;
    if (beta < 0.0f) {
      return false;
    }

    const float gamma = 1.0f - alpha - beta;
    if (gamma < 0.0f) {
      return false;
    }

    if (!psurface_interaction || !phit) return true;

    *phit = kt;
    psurface_interaction->wo_world = Vec3(-ray.get_direction());
    psurface_interaction->hit_point = p;
    psurface_interaction->n = normal;
    psurface_interaction->pshape = this;

    // Compute tangent vectors
    orthonormal_basis(&(psurface_interaction -> s), &(psurface_interaction -> t),
                      psurface_interaction ->n);

    psurface_interaction -> pmaterial = get_material();

    return true;
  }

  RGB_spectrum Triangle::sample_li(const Surface_interaction & interaction,
                                   const Vec2 & u_sample, Vec3 * pwi_world,
                                   Vec3 * point_on_shape, float * pdf) const
  {
    return RGB_spectrum(0.0f);
  }

  float Triangle::PDF(const Surface_interaction & interaction, const Vec3 & wi_world) const
  {
    return 0.0f;
  }
}
