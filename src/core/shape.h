#ifndef LUX_CORE_SHAPE_H_
#define LUX_CORE_SHAPE_H_

#include <memory>

#include "core/vec3.h"
#include "core/transform.h"
#include "core/rgb_spectrum.h"

namespace lux { struct Vec2; class Ray; class Material; }

//TODO: on le pass only the normal of the point, don't need to pass an interaction!
namespace lux {
  class Shape;

  struct Surface_interaction {
    Vec3 wo_world;
    Vec3 hit_point;
    Vec3 n, t, s;
    std::shared_ptr<Material> pmaterial;
    const Shape *pshape;
  };

  class Shape {
    public:
      Shape(const Transform & object_to_world, std::shared_ptr<Material> pmaterial,
            const RGB_spectrum & emitted_radiance)
        : m_object_to_world(object_to_world), 
          m_pmaterial(pmaterial),
          m_emitted_radiance(emitted_radiance) {}

      Shape(const Shape & shape)
         : m_object_to_world(shape.m_object_to_world),
           m_pmaterial(shape.m_pmaterial),
           m_emitted_radiance(shape.m_emitted_radiance) {}

      virtual ~Shape() = default;
      
      Shape & operator=(const Shape & shape)
      {
        m_object_to_world = shape.m_object_to_world;
        m_pmaterial = shape.m_pmaterial;
        m_emitted_radiance = shape.m_emitted_radiance;

        return *this;
      }

      const Transform & get_object_to_world() const { return m_object_to_world; }
      const std::shared_ptr<Material> get_material() const { return m_pmaterial; }

      virtual RGB_spectrum sample_li(const Surface_interaction & interaction,
                                     const Vec2 & u_sample, Vec3 * pwi_world,
                                     Vec3 * point_on_shape, float * pdf) const = 0;

      virtual float PDF(const Surface_interaction & interaction, const Vec3 & wi_world) const = 0;

      virtual bool intersect(const Ray & ray, float * phit, 
                             Surface_interaction * psurface_interaction) const = 0;

      virtual bool intersect_p(const Ray & ray) const
      {
        return intersect(ray, nullptr, nullptr);
      }

      RGB_spectrum le(const Surface_interaction & interaction, const Vec3 & w) const
      {
        return dot(interaction.n, w) > 0.0f  ? m_emitted_radiance : RGB_spectrum(0.0f);
      }

      bool is_area_light() const { return !m_emitted_radiance.is_black(); }

    protected:
      const RGB_spectrum & get_le() const { return m_emitted_radiance; }
    private:
      Transform m_object_to_world;
      std::shared_ptr<Material> m_pmaterial;
      RGB_spectrum m_emitted_radiance;
  };
}
#endif
