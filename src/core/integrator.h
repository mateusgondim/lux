#ifndef LUX_CORE_INTEGRATOR_H_
#define LUX_CORE_INTEGRATOR_H_

#include "core/rgb_spectrum.h"

namespace lux {
  struct Vec2;
  class Ray;
  struct Surface_interaction;
  class Shape;
  class Sampler;
  class Scene;
}

namespace lux {
  class Integrator {
    public:
      Integrator() = default;
      virtual ~Integrator() {}

      virtual RGB_spectrum li(const Scene & scene, const Ray & ray) = 0;
  };

  RGB_spectrum uniform_sample_one_light(const Scene & scene,
                                        const Surface_interaction & interaction,
                                        Sampler & sampler);

  RGB_spectrum estimate_direct(const Scene & scene,
                               const Surface_interaction & interaction,
                               const Vec2 & scattering_sample,
                               const Shape & light,
                               const Vec2 & light_sample);
}

#endif
