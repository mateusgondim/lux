#ifndef LUX_CORE_BOUNDS2_H_
#define LUX_CORE_BOUNDS2_H_

#include <limits>

#include "core/vec2.h"

namespace lux {
  struct Bounds2 final {
    Bounds2() : p_min(), p_max() 
    {
      const float kmin_num = std::numeric_limits<float>::lowest();
      const float kmax_num = std::numeric_limits<float>::max();

      p_min.x = p_min.y = kmin_num;
      p_max.x = p_max.y = kmax_num;
    }
    Bounds2(const Vec2 & min, const Vec2 & max) : p_min(min), p_max(max) {}
    Bounds2(const Bounds2 & bounds2) = default;
    ~Bounds2() = default;

    Bounds2 & operator=(const Bounds2 & bounds2) = default;


    Vec2 p_min;
    Vec2 p_max;
  };
}
#endif
