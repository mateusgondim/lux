#ifndef LUX_CORE_FILTER_H_
#define LUX_CORE_FILTER_H_

#include "core/vec2.h"

namespace lux {
  inline Vec2 box_filter(const Vec2 & k2D_sample)
  {
    return k2D_sample;
  }

  Vec2 triangle_filter(const Vec2 & k2D_sample);
}
#endif
