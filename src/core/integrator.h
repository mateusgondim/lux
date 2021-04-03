#ifndef LUX_CORE_INTEGRATOR_H_
#define LUX_CORE_INTEGRATOR_H_

#include "core/rgb_spectrum.h"

namespace lux { class Scene; class Ray; }

namespace lux {
  class Integrator {
    public:
      virtual RGB_spectrum li(const Scene & scene, const Ray & ray) = 0;
  }
}

#endif
