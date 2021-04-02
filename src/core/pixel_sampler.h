#ifndef LUX_CORE_PIXEL_SAMPLER_H_
#define LUX_CORE_PIXEL_SAMPLER_H_

#include <cstdint>

#include <vector>

#include "core/sampler.h"
#include "core/vec2.h"
#include "core/rng.h"

namespace lux {
  class Pixel_sampler : public Sampler {
    public:
      Pixel_sampler(const std::uint64_t samples_per_pixel, const unsigned dimensions_per_sample);
      virtual void start_pixel(const int x, const int y) override;
      virtual bool start_next_sample() override;

      virtual float get_1D() override;
      virtual Vec2  get_2D() override;

    protected:
      std::vector<std::vector<float> > m_samples_1D;
      std::vector<std::vector<Vec2> > m_samples_2D;
      RNG m_rng;
    private:
      unsigned m_current_1D_dimension;
      unsigned m_current_2D_dimension;
  };
}

#endif
