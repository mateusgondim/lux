#include "shapes/sphere.h"

#include <cmath>

#include <algorithm>

#include "core/util.h"
#include "core/math.h"
#include "core/vec2.h"
#include "core/vec3.h"
#include "core/transform.h"
#include "core/rgb_spectrum.h"
#include "core/ray.h"

namespace lux {
  bool Sphere::intersect(const Ray & ray, float * phit,
                         Surface_interaction * psurface_interaction) const
  {
    // Transform Ray to Object Space
    const Transform & object_to_world = get_object_to_world();
    const Ray r = object_to_world.apply_inverse_on_ray(ray);

    const Vec3 r_o = r.get_origin();
    const Vec3 r_d = r.get_direction();

    float a = dot(r_d, r_d);
    float b = 2 * dot(r_d, r_o);
    float c = dot(r_o, r_o) - m_radius * m_radius;
    float discriminant = b * b - 4*a*c;

    if (discriminant < 0) {
      return false;
    }

    discriminant = sqrt(discriminant);

    const float q = (b < 0.0f) ?(-.5 * (b - discriminant)) :(-.5 * (b + discriminant));
    float t0 = q / a;
    float t1 = c / q;

    if (t0 > t1) {
      std::swap(t0, t1);
    }

    if((t0 > r.get_t_max()) || (t1 <= 0.0f)) {
      return false;
    }

    float shapeHit = t0;
    if (shapeHit <= 0.0f) {
      shapeHit = t1;
      if (shapeHit > r.get_t_max()) {
        return false;
      }
    }

    if (!psurface_interaction || !phit) return true;

    *phit = shapeHit;
    const Vec3 khit_point = r(shapeHit); // hit point in Obj Space

    // Transform the hit point and normal to world space
    psurface_interaction -> wo_world = Vec3(-ray.get_direction());
    psurface_interaction -> hit_point = object_to_world.apply_on_point(khit_point);
    psurface_interaction -> n = normalize(object_to_world.apply_on_normal(khit_point));
    psurface_interaction -> pshape = this;

    // Compute tangent vectors
    orthonormal_basis(&(psurface_interaction -> s), &(psurface_interaction -> t),
                      psurface_interaction ->n);

    psurface_interaction -> pmaterial = get_material();

    return true;
  }

  RGB_spectrum Sphere::sample_li(const Surface_interaction & interaction, const Vec2 & u_sample,
                                 Vec3 *pwi_world, Vec3 * point_on_shape, float * pdf) const
  {
    const Transform & object_to_world = get_object_to_world();
    const Vec3 korigin_wld = object_to_world.apply_on_point(Vec3(0.0f, 0.0f, 0.0f));
    const float kdistance_squared = distance_squared(interaction.hit_point, korigin_wld);

    // Check if the point is inside the sphere
    if (kdistance_squared - kray_epsilon <= m_radius * m_radius) return RGB_spectrum(0.0f);

    Vec3 r = normalize(korigin_wld - interaction.hit_point);
    Vec3 p, q;
    orthonormal_basis(&p, &q, r);

    // Calculate theta and phi
    const float ksin_theta_max_squared = m_radius * m_radius / kdistance_squared;
    const float kcos_theta_max = std::sqrt(1 - ksin_theta_max_squared);
    const float kcos_theta = (1 - u_sample.x) + u_sample.x * kcos_theta_max;
    const float ksin_theta = std::sqrt(1 - kcos_theta * kcos_theta);
    const float kphi = u_sample.y * 2.0f * kpi; 

    const float dc = distance(interaction.hit_point, korigin_wld);
    const float ds = dc * kcos_theta - 
                     std::sqrt(m_radius * m_radius - dc * dc * ksin_theta * ksin_theta);

    const float kcos_alpha = (dc * dc + m_radius * m_radius - ds * ds) / (2 * dc * m_radius);
    const float ksin_alpha = std::sqrt(1 - kcos_alpha * kcos_alpha);

    const Vec3 normal_wld = ksin_alpha * std::cos(kphi) * (-p) +
                            ksin_alpha * std::sin(kphi) * (-q) + 
                            kcos_alpha * (-r);

    const Vec3 sampled_point_wld = korigin_wld + m_radius * normal_wld;

    *pwi_world = normalize(sampled_point_wld - interaction.hit_point);
    *point_on_shape = sampled_point_wld;
    *pdf = 1.0f / (2.0f * kpi * (1 - kcos_theta_max)); 

    Surface_interaction shape_interaction;
    shape_interaction.hit_point = sampled_point_wld;
    shape_interaction.n = normal_wld;

    return le(shape_interaction, -(*pwi_world));
  }

  float Sphere::PDF(const Surface_interaction & interaction, const Vec3 & wi_world) const
  {
    const Transform & object_to_world = get_object_to_world();
    const Vec3 korigin_wld = object_to_world.apply_on_point(Vec3(0.0f, 0.0f, 0.0f));
    const float kdistance_squared = distance_squared(interaction.hit_point, korigin_wld);

    const float ksin_theta_max_squared = m_radius * m_radius / kdistance_squared;
    const float kcos_theta_max = std::sqrt(1 - ksin_theta_max_squared);

    return 1.0f / (2.0f * kpi * (1 - kcos_theta_max));
  }
}
