#ifndef LUX_CORE_UTIL_H_
#define LUX_CORE_UTIL_H_

#include <cstddef>
#include <cstdlib>

#include "core/math.h"
#include "core/vec2.h"
#include "core/vec3.h"
#include "core/rng.h"

namespace lux {
  const float kshadow_epsilon = 0.0001f;
  const float kray_epsilon = kshadow_epsilon;

  // Shufle the values of an array with count elements of type T.
  template <typename T>
    void shuffle(T * values, const std::size_t count, RNG & rng)
    {
      for (std::size_t i = count - 2; i != 0; --i) {
        std::size_t random_index_before_i = i * rng();
        T temp = values[i + 1];
        values[i + 1] = values[random_index_before_i];
        values[random_index_before_i] = temp;
      }
    }

  inline Vec2 concentric_sample_disk(const Vec2 & uv_sample)
  {
    // Map uniform random numbers to $[-1,1]^2$
    Vec2 uv_offset = 2.f * uv_sample - Vec2(1, 1);

    if (uv_offset.x == 0 && uv_offset.y == 0) return Vec2(0, 0);

    float theta, r;
    if (std::abs(uv_offset.x) > std::abs(uv_offset.y)) {
      r = uv_offset.x;
      theta = kpi_over_four * (uv_offset.y / uv_offset.x);
    } 
    else {
      r = uv_offset.y;
      theta = kpi_over_two - kpi_over_four * (uv_offset.x / uv_offset.y);
    }

    return r * Vec2(std::cos(theta), std::sin(theta));
  }

  inline Vec3 cosine_sample_hemisphere(const Vec2 & u)
  {
    const Vec2 d = concentric_sample_disk(u);
    float z = std::sqrt(std::max((float)0, 1 - d.x * d.x - d.y * d.y));

    return Vec3(d.x, d.y, z);
  }

  inline float power_heuristic(const int nf, const float f_PDF, const int ng, const float g_PDF)
  {
    const float f = nf * f_PDF;
    const float g = ng * g_PDF;
    return (f * f) / (f * f + g * g);
  }
}

#endif
