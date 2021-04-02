#ifndef LUX_MATERIALS_LAMBERTIAN_H_
#define LUX_MATERIALS_LAMBERTIAN_H_

#include "core/rgb_spectrum.h"
#include "core/math.h"
#include "core/vec3.h"
#include "core/material.h"

namespace lux {
  class Lambertian : public Material {
    public:
      Lambertian(const RGB_spectrum &  R) : Material(Material_type::kdiffuse), m_R(R) {}

      RGB_spectrum f(const Vec3 & wo_world, const Vec3 & wi_world) const override
      { 
        return kinv_pi * m_R;
      }
    private:
      RGB_spectrum m_R;
  };
}

#endif
