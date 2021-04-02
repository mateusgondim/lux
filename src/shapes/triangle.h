#ifndef LUX_SHAPES_TRIANGLE_H
#define LUX_SHAPES_TRIANGLE_H

#include <memory>

#include "core/shape.h"
#include "core/vec3.h"
#include "core/error.h"

namespace lux { class RGB_spectrum; class Ray; class Material; class Transform; }

namespace lux {
  class Triangle : public Shape {
    public:
      Triangle(const Transform & object_to_world, std::shared_ptr<Material> pmaterial,
               const RGB_spectrum & emitted_radiance,
               const Vec3 & v1, const Vec3 & v2, const Vec3 & v3)
        : Shape(object_to_world, pmaterial, emitted_radiance), m_v1(v1), m_v2(v2), m_v3(v3) {}

      Triangle(const Triangle & triangle)
          : Shape(triangle), m_v1(triangle.m_v1), m_v2(triangle.m_v2), m_v3(triangle.m_v3) {}

      Triangle & operator=(const Triangle & triangle)
      {
        Shape::operator=(triangle);

        m_v1 = triangle.m_v1;
        m_v2 = triangle.m_v2;
        m_v3 = triangle.m_v3;

        return *this;
      }

      virtual bool intersect(const Ray & ray, float * phit,
                             Surface_interaction *psurface_interation) const override;

      virtual RGB_spectrum sample_li(const Surface_interaction & interaction,
                                     const Vec2 & u_sample, Vec3 * pwi_world,
                                     Vec3 * point_on_shape, float * pdf) const override;

      virtual float PDF(const Surface_interaction & interaction,
                        const Vec3 & wi_world) const override;

      const Vec3 & operator[](const unsigned i) const;
      Vec3 & operator[](const unsigned i);

    private:
      Vec3 m_v1;
      Vec3 m_v2;
      Vec3 m_v3;
  };

  inline const Vec3 & Triangle::operator[](const unsigned i) const 
  {
    ASSERT(i < 3, "Trying to access a non existent triagle vertex");
    if (i == 0) return m_v1;
    if (i == 1) return m_v2;
    return m_v3;
  }

  inline Vec3 & Triangle::operator[](const unsigned i)
  {
    return const_cast<Vec3 &>(static_cast<const Triangle &>(*this)[i]);
  }
}
#endif
