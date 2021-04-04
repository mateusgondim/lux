#ifndef LUX_INTEGRATORS_PATH_TRACER_H_
#define LUX_INTEGRATORS_PATH_TRACER_H_

#include "core/integrator.h"

#include "core/rgb_spectrum.h"

namespace lux { class Ray; class Scene; class Sampler; }

namespace lux {
  class Path_tracer final : public Integrator {
    public:
      Path_tracer(Sampler * psampler, unsigned max_depth);

      Path_tracer(const Path_tracer &) = delete;
      Path_tracer & operator=(const Path_tracer &) = delete;

      virtual RGB_spectrum li(const Scene & scene, const Ray & r) override;

      virtual ~Path_tracer() override;

    private:
      Sampler * m_psampler;
      const unsigned m_kmax_depth;
  };
}

#endif
