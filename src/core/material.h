#ifndef LUX_CORE_MATERIAL_H_
#define LUX_CORE_MATERIAL_H_

#include <cmath>

#include "core/rgb_spectrum.h"
#include "core/vec3.h"
#include "core/vec2.h"
#include "core/util.h"
#include "core/math.h"
#include "core/shape.h"

namespace lux {

  enum Material_type {
    kdiffuse,
    kspecular,
    kglossy
  };

  class Material {
    public:
      Material(const Material_type & material_type) : m_type(material_type), m_s(), m_t(), m_n() {}

      virtual RGB_spectrum f(const Vec3 & wo_world, const Vec3 & wi_world) const = 0;
      virtual RGB_spectrum sample_f(const Vec3 & wo_world, Vec3 * pwi_world, const Vec2 & sample,
                                    float * pdf) const;

      virtual float PDF(const Vec3 & wo_world, const Vec3 & wi_world) const;

      void init_shading_space(const Surface_interaction & surface_interaction)
      {
        m_s = surface_interaction.s;
        m_t = surface_interaction.t;
        m_n = surface_interaction.n;
      }

      Material_type get_type() const { return m_type; }

    protected:
      Vec3 world_to_shading(const Vec3 & v) const;
      Vec3 shading_to_world(const Vec3 & v) const;
      bool same_hemisphere(const Vec3 & wo, const Vec3 & wi) const;
      float cos_theta(const Vec3 & w) const;
      float squared_cos_theta(const Vec3 & w) const;
      float abs_cos_theta(const Vec3 & w) const;
      Vec3 reflect(const Vec3 & wo) const;

    private:
      Material_type m_type;
      Vec3 m_s, m_t, m_n; // Define the local shading space.
  };

  inline bool Material::same_hemisphere(const Vec3 & wo, const Vec3 & wi) const
  {
    return wo.z * wi.z > 0;
  }

  inline Vec3 Material::world_to_shading(const Vec3 & v) const
  {
    return Vec3(v.x * m_s.x + v.y * m_s.y + v.z * m_s.z,
                v.x * m_t.x + v.y * m_t.y + v.z * m_t.z,
                v.x * m_n.x + v.y * m_n.y + v.z * m_n.z);
  }

  inline Vec3 Material::shading_to_world(const Vec3 & v) const
  {
    return Vec3(v.x * m_s.x + v.y * m_t.x + v.z * m_n.x,
                v.x * m_s.y + v.y * m_t.y + v.z * m_n.y,
                v.x * m_s.z + v.y * m_t.z + v.z * m_n.z);
  }

  inline float Material::cos_theta(const Vec3 & w) const
  {
    return w.z;
  }

  inline float Material::squared_cos_theta(const Vec3 & w) const
  {
    return w.z * w.z;
  }

  inline float Material::abs_cos_theta(const Vec3 & w) const
  {
    return std::abs(w.z);
  }

  inline Vec3 Material::reflect(const Vec3 & wo) const
  {
    return Vec3(-wo.x, -wo.y, wo.z);
  }
}

#endif
