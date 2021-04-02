#include "core/vec2.h"

#include <iostream>

namespace lux {
  std::ostream & operator<<(std::ostream & os, const Vec2 & v)
  {
    os << "[" << v.x << ", " << v.y << "]";

    return os;
  }
}
