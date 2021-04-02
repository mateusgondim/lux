#include "materials/mirror.h"

#include "core/rgb_spectrum.h"
#include "core/vec2.h"
#include "core/vec3.h"


namespace lux {

  RGB_spectrum Mirror::sample_f(const Vec3 & wo_world, Vec3 * pwi_world, const Vec2 & sample,
                                float * pdf) const
  {
    Vec3 wo = world_to_shading(wo_world);
    Vec3 wi = reflect(wo);

    *pwi_world = shading_to_world(wi);
    *pdf = 1.0f;

    return m_R / abs_cos_theta(wi);
  }
  
  RGB_spectrum Mirror::f(const Vec3 & wo_world, const Vec3 & wi_world) const
  {
    return RGB_spectrum(0.0f);
  }

  float Mirror::PDF(const Vec3 & wo_world, const Vec3 & wi_world) const
  {
    return 0.0f;
  }
}
