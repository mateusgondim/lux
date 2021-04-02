#include "core/rgb_spectrum.h"

#include <iostream>
#include <iomanip>

namespace lux {

  bool RGB_spectrum::is_black() const
  {
    if (m_rgb[0] == 0.0f && m_rgb[1] == 0.0f && m_rgb[2] == 0.0f) return true;
    return false;
  }

  RGB_spectrum & RGB_spectrum::clamp()
  {
    if (m_rgb[0] > 1.0f) m_rgb[0] = 1.0f;
    if (m_rgb[1] > 1.0f) m_rgb[1] = 1.0f;
    if (m_rgb[2] > 1.0f) m_rgb[2] = 1.0f;

    if (m_rgb[0] < 0.0f) m_rgb[0] = 0.0f;
    if (m_rgb[1] < 0.0f) m_rgb[1] = 0.0f;
    if (m_rgb[2] < 0.0f) m_rgb[2] = 0.0f;

    return *this;
  }

  RGB_spectrum clamp(const RGB_spectrum & spectrum)
  {
    RGB_spectrum s(spectrum);
    return s.clamp();
  }

  bool operator==(const RGB_spectrum & lhs, const RGB_spectrum & rhs)
  {
    if (lhs.m_rgb[0] == rhs.m_rgb[0] &&
        lhs.m_rgb[1] == rhs.m_rgb[1] && 
        lhs.m_rgb[2] == rhs.m_rgb[2]) {
      return true;
    }
    else {
      return false;
    }
  }

  bool operator!=(const RGB_spectrum & lhs, const RGB_spectrum & rhs) 
  {
    return !(lhs == rhs);
  }

  std::ostream & operator<<(std::ostream & os, const RGB_spectrum & spectrum)
  {
    std::streamsize stream_precision = os.precision();
    os.precision(4);

    os << std::fixed;
    os << "[" << std::setw(10) << spectrum.x() << std::setw(10) << spectrum.y() << std::setw(10) 
              << spectrum.z()  << " ]";

    os.precision(stream_precision);
    return os;
  }
}
