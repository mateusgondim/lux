#include "samplers/random.h"

#include <cstdint>

#include "core/vec2.h"
#include "core/rng.h"

namespace lux {
  Random_sampler::Random_sampler(const std::uint64_t samples_per_pixel)
      : Sampler(samples_per_pixel), rng() {}

  float Random_sampler::get_1D() { return rng(); }
  Vec2 Random_sampler::get_2D() { return Vec2(rng(), rng()); }
}
