#include "core/material.h"

#include "core/vec2.h"
#include "core/vec3.h"
#include "core/rgb_spectrum.h"

namespace lux {

  RGB_spectrum Material::sample_f(const Vec3 & wo_world, Vec3 * pwi_world, const Vec2 & sample,
                                  float * pdf) const 
  {
    Vec3 wo = world_to_shading(wo_world);

    Vec3 wi = cosine_sample_hemisphere(sample);
    if (wo.z < 0) wi.z *= -1;

    *pdf = same_hemisphere(wo, wi) ? abs_cos_theta(wi) * kinv_pi : 0;
    *pwi_world = shading_to_world(wi);

    return f(wo, wi);
  }

  float Material::PDF(const Vec3 & wo_world, const Vec3 & wi_world) const
  {
    const Vec3 wo = world_to_shading(wo_world);
    const Vec3 wi = world_to_shading(wi_world);

    return same_hemisphere(wo, wi) ? abs_cos_theta(wi) * kinv_pi : 0;
  }
}
