#ifndef LUX_SAMPLERS_RANDOM_H_
#define LUX_SAMPLERS_RANDOM_H_

#include <cstdint>

#include "core/sampler.h"
#include "core/rng.h"
#include "core/vec2.h"

namespace lux {
  class Random_sampler : public Sampler {
    public:
      Random_sampler(const std::uint64_t samples_per_pixel);

      virtual float get_1D() override;
      virtual Vec2 get_2D() override;
    private:
      RNG rng;
  };
}

#endif
