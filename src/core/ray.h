#ifndef LUX_CORE_RAY_H_
#define LUX_CORE_RAY_H_

#include <limits>
#include <iostream>

#include "core/vec3.h"

namespace lux {
  class Ray {
    public:
      Ray() 
          : m_origin(), m_direction(), m_t_max(std::numeric_limits<float>::infinity()), m_time(0) {}
      Ray(const Vec3 & origin, const Vec3 & direction,
          float t_max = std::numeric_limits<float>::infinity(), float time = 0.0f)
          : m_origin(origin), m_direction(direction), m_t_max(t_max), m_time(time) {}
      Ray(const Ray & ray) = default;
      ~Ray() = default;

      Ray & operator=(const Ray & rhs) = default;
      void set_origin(const Vec3 & origin) { m_origin = origin; }
      void set_direction(const Vec3 & direction) { m_direction = direction; }
      void set_t_max(const float t_max) const { m_t_max = t_max; }
      void set_time(const float time) { m_time = time; }

      Vec3 operator()(const float t) const { return m_origin + t*m_direction; }
      Vec3 get_direction() const { return m_direction; }
      Vec3 get_origin() const { return m_origin; }
      float get_t_max() const { return m_t_max; }
      float get_time() const { return m_time; }

    private:
      Vec3 m_origin;
      Vec3 m_direction;
      mutable float m_t_max;
      float m_time;
  };

  inline std::ostream & operator<<(std::ostream & os, const Ray & r)
  {
    os << "r = " << "{ o = " << r.get_origin() << ", d = " << r.get_direction();

    return os;
  }
}
#endif
