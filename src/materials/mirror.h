#ifndef LUX_MATERIALS_MIRROR_H_
#define LUX_MATERIALS_MIRROR_H_

#include "core/material.h"
#include "core/rgb_spectrum.h"

namespace lux { struct Vec2; struct Vec3; }

namespace lux {
  class Mirror final : public Material {
    public:
      Mirror(const RGB_spectrum & R) : Material(Material_type::kspecular), m_R(R) {}

      virtual RGB_spectrum f(const Vec3 & wo_world, const Vec3 & wi_world) const override;
      virtual RGB_spectrum sample_f(const Vec3 & wo_world, Vec3 * pwi_world, const Vec2 & sample,
                                    float * pdf) const override;
      virtual float PDF(const Vec3 & wo_world, const Vec3 & wi_world) const override;
    private:
      RGB_spectrum m_R;
  };
}

#endif
