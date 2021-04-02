#include "core/vec3.h"

#include <iostream>
#include <iomanip>

namespace lux {
  std::ostream & operator<<(std::ostream & os, const Vec3 & v)
  {
    std::streamsize stream_precision = os.precision();
    os.precision(4);

    os << std::fixed;
    os << "[" << std::setw(10) << v.x << std::setw(10) << v.y << std::setw(10) << v.z  << " ]";

    os.precision(stream_precision);
    return os;
  }
}
