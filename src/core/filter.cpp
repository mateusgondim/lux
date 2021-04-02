#include "core/filter.h"

#include <cmath>

#include "core/vec2.h"

namespace lux {

  Vec2 triangle_filter(const Vec2 & k2D_sample)
  {
    Vec2 filtered_2D_sample;
    const double kx = static_cast<double>(k2D_sample.x);
    const double ky = static_cast<double>(k2D_sample.y);

    if (k2D_sample.x < 0.5f) {
      filtered_2D_sample.x = static_cast<float>(sqrt(2.0 * kx)) - 1.0f; 
    }
    else {
      filtered_2D_sample.x = 1.0f - static_cast<float>(sqrt(2.0 - 2.0 * kx));
    }

    if (k2D_sample.y < 0.5f) {
      filtered_2D_sample.y = static_cast<float>(sqrt(2.0 * ky)) - 1.0f; 
    }
    else {
      filtered_2D_sample.y = 1.0f - static_cast<float>(sqrt(2.0 - 2.0 * ky));
    }

    filtered_2D_sample.x = (filtered_2D_sample.x + 1.0f) / 2.0f;
    filtered_2D_sample.y = (filtered_2D_sample.y + 1.0f) / 2.0f;

    return filtered_2D_sample;
  }

}
