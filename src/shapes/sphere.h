#ifndef LUX_SHAPES_SPHERE_H_
#define LUX_SHAPES_SPHERE_H_

#include <memory>

#include "core/rgb_spectrum.h"
#include "core/shape.h"

namespace lux { struct Vec2; struct Vec3; class Ray; struct Surface_interaction;
                      class Material; class Transform; }

namespace lux {
  class Sphere final : public Shape {
    public:
      Sphere(const Transform & object_to_world, std::shared_ptr<Material> pmaterial,
             const RGB_spectrum & emitted_radiance, const float kradius)
          : Shape(object_to_world, pmaterial, emitted_radiance), m_radius(kradius) {}

      Sphere(const Sphere & sphere) : Shape(sphere), m_radius(sphere.m_radius) {}

      Sphere & operator=(const Sphere & sphere)
      {
        Shape::operator=(sphere);
        m_radius = sphere.m_radius;

        return *this;
      }

      virtual bool intersect(const Ray & ray, float * phit, 
                             Surface_interaction * psurface_interaction) const override;

      virtual RGB_spectrum sample_li(const Surface_interaction & interaction,
                                     const Vec2 & u_sample, Vec3 * pwi_world,
                                     Vec3 * point_on_shape, float * pdf) const override;

      virtual float PDF(const Surface_interaction & interaction,
                        const Vec3 & wi_world) const override;

      float get_radius() const { return m_radius; }
      
    private:
      float m_radius;
  };
}

#endif
